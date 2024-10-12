
// main function
// run this to start the client
public class Main {
    public static void main(String[] args) {
        try {
            CLI cli = new CLI();
            cli.start();
        }catch (Exception e) {
            System.out.println("General exception occurred: " + e.getMessage());
            return;
        }

    }
}
