import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public class RequestBuilder {
    // Query flights by source and destination
    public static byte[] buildQueryFlights(String source, String destination) {
        byte[] srcBytes = source.getBytes(StandardCharsets.US_ASCII);
        byte[] destBytes = destination.getBytes(StandardCharsets.US_ASCII);
        ByteBuffer buffer = ByteBuffer.allocate(4 + srcBytes.length + 4 + destBytes.length);  // 1 int + buffer + 1 int + buffer
        buffer.putInt(srcBytes.length);
        buffer.put(srcBytes);
        buffer.putInt(destBytes.length);
        buffer.put(destBytes);
        return buffer.array();
    }

    // Query flight details by id
    public static byte[] buildQueryFlightDetails(int flightId) {
        ByteBuffer buffer = ByteBuffer.allocate(4);   // 1 int
        buffer.putInt(flightId);
        return buffer.array();
    }

    // Reserve seats by id and number of seats
    public static byte[] buildReserveSeats(int flightId, int numSeats) {
        ByteBuffer buffer = ByteBuffer.allocate(8);  // 2 int
        buffer.putInt(flightId);
        buffer.putInt(numSeats);
        return buffer.array();
    }

//      Register for seat avail updates
    public static byte[] buildSeatRegister(int flightId, int monitor) {
        ByteBuffer buffer = ByteBuffer.allocate(8);  // 2 int
        buffer.putInt(flightId);
        buffer.putInt(monitor);
        return buffer.array();
    }

    // Query flights by source and airfare range

    public static byte[] buildQueryFlightsByAirfareRange(String source, double minFare, double maxFare) {
        byte[] srcBytes = source.getBytes();
        ByteBuffer buffer = ByteBuffer.allocate(4 + srcBytes.length + 16);  // 1 int + 2 double
        buffer.putInt(srcBytes.length);
        buffer.put(srcBytes);
        buffer.putDouble(minFare);
        buffer.putDouble(maxFare);
        return buffer.array();
    }

//    Delay flight by id and duration in Ms
    public static byte[] buildDelayFlights(int flightId, int delayMs) {
        ByteBuffer buffer = ByteBuffer.allocate(8);  // 2 int
        buffer.putInt(flightId);
        buffer.putInt(delayMs);
        return buffer.array();
    }


}
