import java.net.SocketException;
import java.net.UnknownHostException;
import java.net.SocketTimeoutException;

public class CLI {
    private Client client;
    private InputHandler inputHandler;
//    private String serverIP = "10.91.155.181";
    private String serverIP = "20.2.218.136";  //server IP
    private int serverPort = 3000;  //server port

    private int totalTimes = 5;  // timeout count
    private int retryInterval = 5000;  // timeout interval

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
        client.transmitRequest();      //+1 for requestId

        while (currentTimes < totalTimes) {
            try {
                // transmit the message
                client.sendRequest(msgType, serviceType, msgBody);
                System.out.println("Sent message to " + serverIP + ":" + serverPort);
                currentTimes++;

                // wait for the response from server
                byte[] rawResponse = client.receiveReply();
                if (rawResponse != null) {
                    ReplyData response = client.unmarshalReply(rawResponse);
                    System.out.println("Received response: " + response.toString());
                    if(response.getFlights() == null){
                        return false;
                    }
                    return true;  // receive the response successfully
                }
            } catch (SocketTimeoutException e) {
                // timeout. retransmitting
                System.out.println("No response received, retransmitting...");
            } catch (IllegalStateException e) {
                // wrong requestId
                System.out.println(e.getMessage()+", retransmitting...");
                try {
                    Thread.sleep(retryInterval);  // 等待 5 秒后再次重传
                } catch (InterruptedException e1) {
                    System.out.println("Thread interrupted: " + e1.getMessage());
                    return false;
                }
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

//            try {
//                Thread.sleep(retryInterval);  // 等待 1 秒后再次重传
//            } catch (InterruptedException e) {
//                System.out.println("Thread interrupted: " + e.getMessage());
//                return;
//            }
        }

        // after 5 tries for retransmission, return false
        System.out.println("Operation timed out, please try again later.");
        System.out.println("--------------------------------------------------------");
        System.out.println();
        return false;
    }



    // Invoke RequestBuilder build buffer and send it
    private void queryFlights() throws Exception {
        String source = inputHandler.getStringInput("Enter source: ");
        String destination = inputHandler.getStringInput("Enter destination: ");
        byte[] requestPayload = RequestBuilder.buildQueryFlights(source, destination);

        // invoke sendRequest send msg
        requestTimeoutMonitor((byte) 0, (byte)1, requestPayload);  // serviceType is 1
    }

    private void queryFlightDetails() throws Exception {
        int flightId = inputHandler.getIntInput("Enter flight ID: ");
        byte[] requestPayload = RequestBuilder.buildQueryFlightDetails(flightId);

        requestTimeoutMonitor((byte) 0, (byte)2, requestPayload);  // serviceType is 2
    }

    private void reserveSeats() throws Exception {
        int flightId = inputHandler.getIntInput("Enter flight ID: ");
        int numSeats = inputHandler.getIntInput("Enter number of seats: ");
        byte[] requestPayload = RequestBuilder.buildReserveSeats(flightId, numSeats);

        requestTimeoutMonitor((byte) 0, (byte)3, requestPayload);  // serviceType is 3
    }

    private void seatRegister() throws Exception {
        int flightId = inputHandler.getIntInput("Enter flight ID: ");
        int monitor = inputHandler.getIntInput("Enter time interval (in ms): ");
        byte[] requestPayload = RequestBuilder.buildSeatRegister(flightId, monitor);

        if (requestTimeoutMonitor((byte) 0, (byte)4, requestPayload))   // serviceType is 4
            client.callbackMonitor(monitor + 1000);
    }

    private void queryFlightsByAirfareRange() throws Exception {
        String source = inputHandler.getStringInput("Enter source: ");
        double minFare = inputHandler.getDoubleInput("Enter minimum flight fare: ");
        double maxFare = inputHandler.getDoubleInput("Enter maximum flight fare: ");
        byte[] requestPayload = RequestBuilder.buildQueryFlightsByAirfareRange(source, minFare, maxFare);

        requestTimeoutMonitor((byte) 0, (byte)5, requestPayload);  // serviceType is 5
    }

    private void delayFlights() throws Exception {
        int flightId = inputHandler.getIntInput("Enter flight ID: ");
        int delayMs = inputHandler.getIntInput("Enter delay time (in ms): ");
        byte[] requestPayload = RequestBuilder.buildDelayFlights(flightId, delayMs);

        requestTimeoutMonitor((byte) 0, (byte)6, requestPayload);  // serviceType is 6
    }
}
