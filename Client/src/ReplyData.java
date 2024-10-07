import java.util.Arrays;

public class ReplyData {
    private byte msgType;
    private int requestId;
    private int serviceType;
//    private boolean isSuccess;
    private String message;
    private FlightData[] flights;

    // 成功响应
    public ReplyData(byte msgType, int requestId, int serviceType, String message, FlightData[] flights) {
        this.msgType = msgType;
        this.requestId = requestId;
        this.serviceType = serviceType;
//        this.isSuccess = isSuccess;
        this.message = message;
        this.flights = flights;
    }

    // 构造方法用于处理单个航班，将其转化为数组
    public ReplyData(byte msgType, int requestId, int serviceType, String message, FlightData flightData) {
        this.msgType = msgType;
        this.requestId = requestId;
        this.serviceType = serviceType;
        this.message = message;
        this.flights = new FlightData[]{flightData};  // 单个航班数据转化为数组
    }

    // 错误响应
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
}
