import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Scanner;
import java.net.SocketTimeoutException;

public class CLI {
    private Client client;
    private InputHandler inputHandler;
//    private String serverIP = "192.168.1.104";
    private String serverIP = "20.2.218.136";
    private int serverPort = 3000;

    private int totalTimes = 5;  // 超时重传次数
//    private int retryInterval = 1000;  // 超时重传间隔

    public CLI() throws Exception {
        this.client = new Client(serverIP, serverPort);
        this.inputHandler = new InputHandler();
    }

    public void start() {
        while (true) {
            showMenu();
            int choice = inputHandler.getIntInput("Enter your choice: ");

            try {
                switch (choice) {
                    case 1:
                        queryFlights();
                        break;
                    case 2:
                        queryFlightDetails();
                        break;
                    case 3:
                        reserveSeats();
                        break;
                    case 4:
                        seatRegister();
                        break;
                    case 5:
                        queryFlightsByAirfareRange();
                        break;
                    case 6:
                        delayFlights();
                        break;
                    case 0:
                        client.close();
                        return;
                    default:
                        System.out.println("Invalid option.");
                }
            } catch (Exception e) {
                System.out.println("An error occurred: " + e.getMessage());
            }
        }
    }

    private void showMenu() {
        System.out.println("*******************************************************");
        System.out.println("Welcome to flight service, please select the function:");
        System.out.println("1. Query Flights by Source and Destination");
        System.out.println("2. Query Flight Details by ID");
        System.out.println("3. Reserve Seats by ID and Number");
        System.out.println("4. Register for Seat Available Updates");
        System.out.println("5. Query Flights by Source and Airfare Range");
        System.out.println("6. Delay Flight by ID and Duration in Ms");
        System.out.println("0. Exit");
//        System.out.print("Enter your choice: ");
    }


    private boolean requestTimeoutMonitor(byte msgType, byte serviceType, byte[] msgBody) {
        int currentTimes = 0;
        client.transmitRequest();

        while (currentTimes < totalTimes) {
            try {
                // 发送消息
                client.sendRequest(msgType, serviceType, msgBody);
                System.out.println("Sent message to " + serverIP + ":" + serverPort);
                currentTimes++;

                // 等待接收服务器响应
                byte[] rawResponse = client.receiveReply();
                if (rawResponse != null) {
                    ReplyData response = client.unmarshalReply(rawResponse);
                    System.out.println("Received response: " + response.toString());
                    if(response.getFlights() == null){
                        return false;
                    }
                    return true;  // 成功收到响应后，退出方法
                }
            } catch (SocketTimeoutException e) {
                // 如果超时但未达到总超时，则重传请求
                System.out.println("No response received, retransmitting...");
            } catch (UnknownHostException e) {
                System.out.println("Invalid server IP address: " + e.getMessage());
                return false;
            } catch (SocketException e) {
                System.out.println("Socket error: " + e.getMessage());
                return false;
            } catch (Exception e) {
                System.out.println("General exception occurred: " + e.getMessage());
                return false;
            }

//            // 如果在这次循环中没有收到响应，等待 retryInterval 时间后重试
//            try {
//                Thread.sleep(retryInterval);  // 等待 1 秒后再次重传
//            } catch (InterruptedException e) {
//                System.out.println("Thread interrupted: " + e.getMessage());
//                return;
//            }
        }

        // 重传5次后仍未收到响应，返回超时错误
        System.out.println("Operation timed out, please try again later.");
        System.out.println("--------------------------------------------------------");
        System.out.println();
        return false;
    }



    // 调用 RequestBuilder 构建请求并发送
    private void queryFlights() throws Exception {
        String source = inputHandler.getStringInput("Enter source: ");
        String destination = inputHandler.getStringInput("Enter destination: ");
        byte[] requestPayload = RequestBuilder.buildQueryFlights(source, destination);

        // 调用 sendRequest 发送数据
        requestTimeoutMonitor((byte) 0, (byte)1, requestPayload);  // serviceType 为 1
    }

    private void queryFlightDetails() throws Exception {
        int flightId = inputHandler.getIntInput("Enter flight ID: ");
        byte[] requestPayload = RequestBuilder.buildQueryFlightDetails(flightId);

        // 调用 sendRequest 发送数据
        requestTimeoutMonitor((byte) 0, (byte)2, requestPayload);  // serviceType 为 2
    }

    private void reserveSeats() throws Exception {
        int flightId = inputHandler.getIntInput("Enter flight ID: ");
        int numSeats = inputHandler.getIntInput("Enter number of seats: ");
        byte[] requestPayload = RequestBuilder.buildReserveSeats(flightId, numSeats);

        // 调用 sendRequest 发送数据
        requestTimeoutMonitor((byte) 0, (byte)3, requestPayload);  // serviceType 为 3
    }

    private void seatRegister() throws Exception {
        int flightId = inputHandler.getIntInput("Enter flight ID: ");
        int monitor = inputHandler.getIntInput("Enter time interval: ");
        byte[] requestPayload = RequestBuilder.buildSeatRegister(flightId, monitor);

        if (requestTimeoutMonitor((byte) 0, (byte)4, requestPayload))   // serviceType 为 4
            client.callbackMonitor(monitor);
    }

    private void queryFlightsByAirfareRange() throws Exception {
        String source = inputHandler.getStringInput("Enter source: ");
        double minFare = inputHandler.getDoubleInput("Enter minimum flight fare: ");
        double maxFare = inputHandler.getDoubleInput("Enter maximum flight fare: ");
        byte[] requestPayload = RequestBuilder.buildQueryFlightsByAirfareRange(source, minFare, maxFare);

        requestTimeoutMonitor((byte) 0, (byte)5, requestPayload);  // serviceType 为 5
    }

    private void delayFlights() throws Exception {
        int flightId = inputHandler.getIntInput("Enter flight ID: ");
        int delayMs = inputHandler.getIntInput("Enter delay time: ");
        byte[] requestPayload = RequestBuilder.buildDelayFlights(flightId, delayMs);

        requestTimeoutMonitor((byte) 0, (byte)6, requestPayload);  // serviceType 为 6
    }
}
