import java.net.UnknownHostException;
import java.net.SocketException;
import java.time.Duration;
import java.time.LocalDateTime;

public class ClientTest {
    public static void main(String[] args) {
        try {
            // 创建客户端对象
            Client client = new Client("192.168.1.104", 3000);
            LocalDateTime sendT  = LocalDateTime.now();

            // 发送消息
            String msg = "Hello, this is a test message from Java client.";
            byte[] sendData = msg.getBytes();
            client.sendRequest((byte)1,(byte)1, sendData);

            // 接收服务器响应
            byte[] response = client.receiveReply();
            LocalDateTime recT  = LocalDateTime.now();
            if (response != null) {
                System.out.println("Received response: " + response);
                System.out.println("Round Time: " + Duration.between(sendT, recT));
            }



            // 关闭客户端
            client.close();
        } catch (UnknownHostException e) {
            System.out.println("Invalid server IP address: " + e.getMessage());
        } catch (SocketException e) {
            System.out.println("Socket error: " + e.getMessage());
        } catch (Exception e) {
            // 捕获所有其他的 Exception 类型
            System.out.println("General exception occurred: " + e.getMessage());
        }
    }
}
