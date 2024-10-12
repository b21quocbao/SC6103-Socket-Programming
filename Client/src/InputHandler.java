import java.util.Scanner;

// handle the input information
public class InputHandler {
    private Scanner scanner;

    public InputHandler() {
        this.scanner = new Scanner(System.in);
    }

    // string input
    public String getStringInput(String prompt) {
        System.out.print(prompt);
        return scanner.nextLine();
//        while(true){
//            System.out.print(prompt);
//            String input = scanner.nextLine();
//            if ("\n".equals(input))
//                return input;
//            System.out.println("Invalid input. Please try again.");
//        }
    }

    // int input
    public int getIntInput(String prompt) {
        while (true) {
            System.out.print(prompt);
            try {
                return Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("Invalid input. Please enter a valid number.");
            }
        }
    }

    // double input
    public double getDoubleInput(String prompt) {
        while (true) {
            System.out.print(prompt);
            try {
                return Double.parseDouble(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("Invalid input. Please enter a valid decimal number.");
            }
        }
    }
}
