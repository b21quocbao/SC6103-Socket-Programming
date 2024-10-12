import java.util.Arrays;

// store the response data
public class ReplyData {
    private byte msgType;
    private int requestId;
    private int serviceType;
//    private boolean isSuccess;
    private String message;
    private FlightData[] flights;

    // response is successful, for multiple flights
    public ReplyData(byte msgType, int requestId, int serviceType, String message, FlightData[] flights) {
        this.msgType = msgType;
        this.requestId = requestId;
        this.serviceType = serviceType;
//        this.isSuccess = isSuccess;
        this.message = message;
        this.flights = flights;
    }

    // response is successful, for single flights
    public ReplyData(byte msgType, int requestId, int serviceType, String message, FlightData flightData) {
        this.msgType = msgType;
        this.requestId = requestId;
        this.serviceType = serviceType;
        this.message = message;
        this.flights = new FlightData[]{flightData};  // 单个航班数据转化为数组
    }

    // response fail
    public ReplyData(byte msgType, int requestId, int serviceType, String message) {
        this.msgType = msgType;
        this.requestId = requestId;
        this.serviceType = serviceType;
        this.message = message;
    }


    @Override
    public String toString() {
        if (flights == null)
            return "ReplyData { message: " + message + " }";
        else if (flights.length == 1) {
            return "ReplyData { message: " + message +
                    "\nflights information: " + flights[0].toString() + " }";
        }
        return "ReplyData { message: " + message +
                "\nflights information: " + Arrays.toString(flights) + " }";
    }

    public byte getMsgType() {
        return msgType;
    }

    public FlightData[] getFlights() {
        return flights;
    }

    public int getRequestId() {
        return requestId;
    }
}
