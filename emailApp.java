package EmailApp;
import java.util.Scanner;
public class emailApp {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Enter your name first name: ");
        String firstName;
        firstName = sc.nextLine();
        
        System.out.println("Enter your last name: ");
        String lastName;
        lastName = sc.nextLine();
        email em1 = new email(firstName, lastName);
        System.out.println(em1.showInfo());

        sc.close();
    }
}
