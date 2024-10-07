import com.sun.jdi.BooleanType;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.Date;

public class ResponseParser {

    public static ReplyData parseResponse(byte[] data) throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(data);

        // 解析报文头部
        byte msgType = buffer.get();      // 1字节: 消息类型 (0表示请求，1表示响应)
        int requestId = buffer.get();       // 1字节: 请求ID
        int serviceType = buffer.get();     // 1字节: 服务类型

        //检验requestId是否收到过，收到才++requestId
//        if(requestId == client.getRequestId()){
//            client.recRequest();
//        }

        // 解析msgBody
        if (msgType == 1) { // 如果是响应
            return parseMsgBody(buffer, msgType, requestId, serviceType);
        } else {
            throw new IllegalStateException("Unexpected msgType: " + msgType);
        }
    }

    // 解析msgBody，根据serviceType选择不同的解析方式
    private static ReplyData parseMsgBody(ByteBuffer buffer, byte msgType, int requestId, int serviceType) throws Exception {
        // 解析msgBody中的成功/错误标志
        byte successFlag = buffer.get(); // 1字节: 成功(1)或错误(0)
        String message = readString(buffer);

        if (successFlag == 0) {
            // 错误响应
            return new ReplyData(msgType, requestId, serviceType, message);
        } else if (successFlag == 1) {
            // 成功响应，根据服务类型解析响应体
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

    // 解析服务类型1: Query Flights by Source and Destination
    private static ReplyData parseQueryFlights(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int numFlights = buffer.getInt();  // 航班数量
        FlightData[] flights = new FlightData[numFlights];

        for (int i = 0; i < numFlights; i++) {
            int flightId = buffer.getInt();
            String src = readString(buffer);  // 读取出发地
            String dest = readString(buffer); // 读取目的地
            Date dep = readDep(buffer); // 读取出发时间
            double fare = readDouble(buffer); // 读取票价
            int seatAvail = buffer.getInt(); // 读取可用座位数

            flights[i] = new FlightData(flightId, src, dest, dep, fare, seatAvail);
        }

        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // 解析服务类型2: Query Flight Details by ID
    private static ReplyData parseFlightDetails(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int flightId = buffer.getInt();  // 航班ID
        String src = readString(buffer);  // 读取出发地
        String dest = readString(buffer); // 读取目的地
        Date dep = readDep(buffer); // 读取出发时间
        double fare = readDouble(buffer); // 读取票价
        int seatAvail = buffer.getInt(); // 读取可用座位数

//        FlightData[] flights = new FlightData[1];
        FlightData flights = new FlightData(flightId, src, dest, dep, fare, seatAvail);

        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // 解析服务类型3: Reserve Seats by ID and Number
    private static ReplyData parseReserveSeats(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int flightId = buffer.getInt();  // 航班ID
        String src = readString(buffer);  // 读取出发地
        String dest = readString(buffer); // 读取目的地
        Date dep = readDep(buffer); // 读取出发时间
        double fare = readDouble(buffer); // 读取票价
        int seatAvail = buffer.getInt(); // 读取更新后的座位数

//        FlightData[] flights = new FlightData[1];
//        flights[0] = new FlightData(flightId, src, dest, dep, fare, updatedSeatAvail);
        FlightData flights = new FlightData(flightId, src, dest, dep, fare, seatAvail);
        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // 解析服务类型4: Register for seat avail updates
    private static ReplyData parseSeatRegister(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int flightId = buffer.getInt();  // 航班ID
        String src = readString(buffer);  // 读取出发地
        String dest = readString(buffer); // 读取目的地
        Date dep = readDep(buffer); // 读取出发时间
        double fare = readDouble(buffer); // 读取票价
        int seatAvail = buffer.getInt(); // 读取更新后的座位数

        FlightData flights = new FlightData(flightId, src, dest, dep, fare, seatAvail);
        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // 解析服务类型5: Query flights by source and airfare range
    private static ReplyData parseQueryFlightsByAirfareRange(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int numFlights = buffer.getInt();  // 航班数量
        FlightData[] flights = new FlightData[numFlights];

        for (int i = 0; i < numFlights; i++) {
            int flightId = buffer.getInt();
            String src = readString(buffer);  // 读取出发地
            String dest = readString(buffer); // 读取目的地
            Date dep = readDep(buffer); // 读取出发时间
            double fare = readDouble(buffer); // 读取票价
            int seatAvail = buffer.getInt(); // 读取可用座位数

            flights[i] = new FlightData(flightId, src, dest, dep, fare, seatAvail);
        }

        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }

    // 解析服务类型6: Delay flight by id and duration in Ms
    private static ReplyData parseDelayFlights(ByteBuffer buffer, byte msgType, int requestId, int serviceType, String message) {
        int flightId = buffer.getInt();  // 航班ID
        String src = readString(buffer);  // 读取出发地
        String dest = readString(buffer); // 读取目的地
        Date dep = readDep(buffer); // 读取更新后的出发时间
        double fare = readDouble(buffer); // 读取票价
        int seatAvail = buffer.getInt(); // 读取座位数

        FlightData flights = new FlightData(flightId, src, dest, dep, fare, seatAvail);

        return new ReplyData(msgType, requestId, serviceType, message, flights);
    }


    // 辅助方法：从ByteBuffer中读取字符串
    private static String readString(ByteBuffer buffer) {
        int strLen = buffer.getInt();  // 读取字符串的长度
        byte[] strBytes = new byte[strLen];
        buffer.get(strBytes);  // 读取字符串字节
        return new String(strBytes, StandardCharsets.US_ASCII);  // 将字节数组转换为字符串
    }

    private static Double readDouble(ByteBuffer buffer) {
        byte[] fareBytes = new byte[8];
        buffer.get(fareBytes);
        ByteBuffer fareBuffer = ByteBuffer.wrap(fareBytes).order(ByteOrder.LITTLE_ENDIAN);
        double fareFull = fareBuffer.getDouble(); // 读取票价
        double fare = Math.round(fareFull * 100.0) / 100.0;
        return fare;
    }

    private static Date readDep(ByteBuffer buffer) {
        int dep = buffer.getInt(); // 读取出发时间
        Date depDate = new Date((long) dep * 1000);
        return depDate;
    }
}
