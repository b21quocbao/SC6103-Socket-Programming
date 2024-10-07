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
        this.clientSocket = new DatagramSocket();  // 随机创建UDP socket
        this.requestID = 0;
        // 设置超时时间
        clientSocket.setSoTimeout(1000);  // 处理超时重传逻辑
    }


    // 发送消息
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

    // 接收消息
    public byte[]  receiveReply() throws Exception {
        byte[] receiveData = new byte[1024];
        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);

        clientSocket.receive(receivePacket);
        //get the real length
        int dataLength = receivePacket.getLength();
        byte[] actualData = new byte[dataLength];
        System.arraycopy(receivePacket.getData(), 0, actualData, 0, dataLength);

        return actualData;

    }

    public ReplyData unmarshalReply(byte[] receivedBytes) throws Exception {
        return ResponseParser.parseResponse(receivedBytes);
    }

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

    public void transmitRequest() {
        requestID++;
    }



    // 关闭客户端
    public void close() {
        if (clientSocket != null && !clientSocket.isClosed()) {
            clientSocket.close();
        }
    }


}
