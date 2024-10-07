import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.time.Duration;
import java.time.LocalDateTime;

public class CommunicationTest {
    public static void main(String[] args) {
        // 服务器的IP地址和端口
        String serverIp = "20.2.209.152";
        int serverPort = 3000;

        // 要发送的测试消息
        String message = "Hello, this is a test message from client.";

        DatagramSocket clientSocket = null;
        try {
            // 创建UDP socket
            clientSocket = new DatagramSocket();

            // 获取服务器地址
            InetAddress serverAddress = InetAddress.getByName(serverIp);

            // 将消息转换为字节数组
            byte[] sendData = message.getBytes();

            // 创建要发送的UDP包
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, serverAddress, serverPort);

            LocalDateTime sendT  = LocalDateTime.now();
            // 发送数据包
            clientSocket.send(sendPacket);
            System.out.println("Sent message to " + serverIp + ":" + serverPort);

            // 设置接收超时（可选）
            clientSocket.setSoTimeout(5000);

            // 准备接收服务器响应
            byte[] receiveData = new byte[1024];
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);

            // 接收服务器的响应
            clientSocket.receive(receivePacket);
            String response = new String(receivePacket.getData(), 0, receivePacket.getLength());
            LocalDateTime recT  = LocalDateTime.now();
            System.out.println("Received response from server: " + response);
            System.out.println("Round Time: " + Duration.between(sendT, recT));
        } catch (Exception e) {
            System.out.println("An error occurred: " + e.getMessage());
        } finally {
            if (clientSocket != null && !clientSocket.isClosed()) {
                clientSocket.close();
            }
        }
    }
}
