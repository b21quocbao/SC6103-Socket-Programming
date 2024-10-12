import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;
import java.nio.ByteBuffer;
import java.util.Arrays;

public class Client {
    private InetAddress serverIP;
    private int serverPort;
    private DatagramSocket clientSocket;

    private byte requestID;


    public Client(String serverIp, int serverPort) throws Exception {
        this.serverIP = InetAddress.getByName(serverIp);
        this.serverPort = serverPort;
        this.clientSocket = new DatagramSocket();  // build a UDP socket
        this.requestID = 0;
        // timeout interval
        clientSocket.setSoTimeout(5000);
    }


    // send Msg
    public void sendRequest(byte msgType, byte serviceType, byte[] msgBody) throws Exception {
        byte reqId = getRequestId();
        ByteBuffer buffer = ByteBuffer.allocate(3 + msgBody.length);
        buffer.put(msgType);
        buffer.put(reqId);
        buffer.put(serviceType);
        buffer.put(msgBody);

        byte[] sendData = buffer.array();
        System.out.println("Byte array as numbers: " + Arrays.toString(sendData));
        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, serverIP, serverPort);
        clientSocket.send(sendPacket);
    }

    // receive Msg
    public byte[]  receiveReply() throws Exception {
        byte[] receiveData = new byte[1024];
        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);

        clientSocket.receive(receivePacket);
        //get the real length
        int dataLength = receivePacket.getLength();
        byte[] actualData = new byte[dataLength];
        System.arraycopy(receivePacket.getData(), 0, actualData, 0, dataLength);
//        System.out.println("Receive byte array as numbers: " + Arrays.toString(actualData));

        return actualData;

    }

    public ReplyData unmarshalReply(byte[] receivedBytes) throws Exception {
        return ResponseParser.parseResponse(receivedBytes, this);
    }

    // callback function
    public void callbackMonitor(int interval) throws Exception {
        long startTime = System.currentTimeMillis();
        System.out.println("Waiting for callback from server...");
        while (System.currentTimeMillis() - startTime < interval) {
            try {
                byte[] data = receiveReply();
                ReplyData response = unmarshalReply(data);
                System.out.println("Received response: " + response.toString());

                if(response.getFlights() == null){
                    break;
                }
            } catch (SocketTimeoutException e) {
                continue;
            } catch (Exception e) {
                System.err.println("Error receiving data: " + e.getMessage());
                break;
            }
        }
        System.out.println("The callback is over.");
        System.out.println("--------------------------------------------------------");
    }


    public byte getRequestId() {
        return requestID;
    }

    //requestID ++
    public void transmitRequest() {
        requestID++;
    }



    public void close() {
        if (clientSocket != null && !clientSocket.isClosed()) {
            clientSocket.close();
        }
    }


}
