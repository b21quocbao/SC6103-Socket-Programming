import com.sun.jdi.BooleanType;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.Date;

public class ResponseParser {

    public static ReplyData parseResponse(byte[] data, Client client) throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(data);

        // parser the Msg head
        byte msgType = buffer.get();      // 1byte: msgType, 1 is reply, 0 is request
        int requestId = buffer.get();       // 1byte: requestId
        int serviceType = buffer.get();     // 1byte: serviceType

        //Check the requestId, for network congestion
        if(requestId != client.getRequestId() && requestId != 0){
            throw new IllegalStateException("Unexpected requestId: " + requestId);
        }

        // parser the Msg body
        if (msgType == 1) { // if it is response
            return parseMsgBody(buffer, msgType, requestId, serviceType);
        } else {
            throw new IllegalStateException("Unexpected msgType: " + msgType);
        }
    }

    // Parser Msg body，in terms of serviceType to choose different funciton
    private static ReplyData parseMsgBody(ByteBuffer buffer, byte msgType, int requestId, int serviceType) throws Exception {
        // Parser whether the execution is successful
        byte successFlag = buffer.get(); // 1字节: 成功(1)或错误(0)
        String message = readString(buffer);

        if (successFlag == 0) {
            // failure execution
            return new ReplyData(msgType, requestId, serviceType, message);
        } else if (successFlag == 1) {
            // successful execution
            switch (serviceType) {
                case 1: // Query Flights by Source and Destination
                    return parseQueryFlights(buffer, msgType, requestId, serviceType, message);
                case 2: // Query Flight Details by ID
                    return parseFlightDetails(buffer, msgType, requestId, serviceType, message);
                case 3: // Reserve Seats by ID and Number
                    return parseReserveSeats(buffer, msgType, requestId, serviceType, message);
                case 4: // Register for seat avail updates
                    return parseSeatRegister(buffer, msgType, requestId, serviceType, message);
                case 5: // Query flights by source and airfare range
                    return parseQueryFlightsByAirfareRange(buffer, msgType, requestId, serviceType, message);
                case 6: // Delay flight by id and duration in Ms
                    return parseDelayFlights(buffer, msgType, requestId, serviceType, message);
                default:
                    throw new IllegalArgumentException("Unsupported serviceType: " + serviceType);
            }
        } else {
            throw new IllegalStateException("Invalid success flag: " + successFlag);
        }
    }

    // Parser service1: Query Flights by Source and Destination
    private static ReplyData parseQueryFlights(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int numFlights = buffer.getInt();  // number of flights
        FlightData[] flights = new FlightData[numFlights];

        for (int i = 0; i < numFlights; i++) {
            int flightId = buffer.getInt();  // flight Id
            String src = readString(buffer);  // Source
            String dest = readString(buffer); // Destination
            Date dep = readDep(buffer); // Departure time
            double fare = readDouble(buffer); // Flight fare
            int seatAvail = buffer.getInt(); // number of seats

            flights[i] = new FlightData(flightId, src, dest, dep, fare, seatAvail);
        }

        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // Parser service2: Query Flight Details by ID
    private static ReplyData parseFlightDetails(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int flightId = buffer.getInt();
        String src = readString(buffer);
        String dest = readString(buffer);
        Date dep = readDep(buffer);
        double fare = readDouble(buffer);
        int seatAvail = buffer.getInt();

//        FlightData[] flights = new FlightData[1];
        FlightData flights = new FlightData(flightId, src, dest, dep, fare, seatAvail);

        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // Parser service3: Reserve Seats by ID and Number
    private static ReplyData parseReserveSeats(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int flightId = buffer.getInt();
        String src = readString(buffer);
        String dest = readString(buffer);
        Date dep = readDep(buffer);
        double fare = readDouble(buffer);
        int seatAvail = buffer.getInt();

//        FlightData[] flights = new FlightData[1];
//        flights[0] = new FlightData(flightId, src, dest, dep, fare, updatedSeatAvail);
        FlightData flights = new FlightData(flightId, src, dest, dep, fare, seatAvail);
        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // Parser service4: Register for seat avail updates
    private static ReplyData parseSeatRegister(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int flightId = buffer.getInt();
        String src = readString(buffer);
        String dest = readString(buffer);
        Date dep = readDep(buffer);
        double fare = readDouble(buffer);
        int seatAvail = buffer.getInt();

        FlightData flights = new FlightData(flightId, src, dest, dep, fare, seatAvail);
        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // Parser service5: Query flights by source and airfare range
    private static ReplyData parseQueryFlightsByAirfareRange(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int numFlights = buffer.getInt();
        FlightData[] flights = new FlightData[numFlights];

        for (int i = 0; i < numFlights; i++) {
            int flightId = buffer.getInt();
            String src = readString(buffer);
            String dest = readString(buffer);
            Date dep = readDep(buffer);
            double fare = readDouble(buffer);
            int seatAvail = buffer.getInt();

            flights[i] = new FlightData(flightId, src, dest, dep, fare, seatAvail);
        }

        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // Parser service6: Delay flight by id and duration in Ms
    private static ReplyData parseDelayFlights(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int flightId = buffer.getInt();
        String src = readString(buffer);
        String dest = readString(buffer);
        Date dep = readDep(buffer);
        double fare = readDouble(buffer);
        int seatAvail = buffer.getInt();

        FlightData flights = new FlightData(flightId, src, dest, dep, fare, seatAvail);

        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }


    // read String
    private static String readString(ByteBuffer buffer) {
        int strLen = buffer.getInt();  // 读取字符串的长度
        byte[] strBytes = new byte[strLen];
        buffer.get(strBytes);  // 读取字符串字节
        return new String(strBytes, StandardCharsets.US_ASCII);  // 将字节数组转换为字符串
    }

    // read Double (need inverse it)
    private static Double readDouble(ByteBuffer buffer) {
        byte[] fareBytes = new byte[8];
        buffer.get(fareBytes);
        ByteBuffer fareBuffer = ByteBuffer.wrap(fareBytes).order(ByteOrder.LITTLE_ENDIAN); // inverse the big endian and little endian
        double fareFull = fareBuffer.getDouble(); // read the fare
        double fare = Math.round(fareFull * 100.0) / 100.0;
        return fare;
    }

    // read Date (need inverse it)
    private static Date readDep(ByteBuffer buffer) {
        int dep = buffer.getInt(); // read departure time
        Date depDate = new Date((long) dep * 1000);
        return depDate;
    }
}
