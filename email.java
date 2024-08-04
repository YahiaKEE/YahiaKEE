package EmailApp;
import java.util.Scanner;
public class email {
    private String firstName;
    private String lastName;
    private String password;
    private String email;
    private String department;
    private int defaultPasswordLength = 10;
    private int mailBoxCapacity = 500;
    private String alternateEmail;
    private String companySuffix = "aeycompany.com";

    //Constructor to receive  first and last name
    public email(String firstName, String lastName) {
        this.firstName = firstName;
        this.lastName = lastName;

        //Call a method asking for the department - return the department
        this.department = setDepartment();

        //Call a method that returns a random password
        this.password = randomPassword(defaultPasswordLength);
        System.out.println("Your password is: " +this.password);

        // Combine elements to generate email
        email = firstName.toLowerCase() + "." + lastName.toLowerCase() + "@" + department + "." + companySuffix;
    }
    // Ask for department
    private String setDepartment() {
        System.out.print("New worker: "+firstName + "\nDepartment Codes: \n1 for Sales\n2 for Acounting\n3 for Development\n0 for none\nEnter your choice: ");
        Scanner in = new Scanner(System.in);
        int depChoice = in.nextInt();
        if(depChoice == 1) {return "sales";}
        else if (depChoice == 2) { return "acct";}
        else if (depChoice == 3) {return "dev";}
        else { return ""; } 
        
    }
    
    //Generate a random password
    private String randomPassword(int length){
        String passwordSet = "ABCDEFGHIJKLMNOPQRSTUVXYZ!@#$%^&*";
        char[] password = new char[length];
        for(int i = 0; i < length; i++) {
            int rand = (int)(Math.random() * passwordSet.length());
            password[i] = passwordSet.charAt(rand);
        }
        return new String (password);
    }
    //Set the mailbox capacity
    public void setMailboxCapacity(int capacity){
        this.mailBoxCapacity = capacity;
    }
    //Set the alternate email
    public void setAlternateEmail(String altEmail) {
        this.alternateEmail = altEmail;
    }
    //Change the password
    public void ChangePassword(String password) {
        this.password = password;
    }

    public int getMailBoxCapacity() {return mailBoxCapacity;}
    public String getAlternateEmail() {return alternateEmail;}
    public String getPassword() {return password;}

    public String showInfo() {
        return "DISPLAY NAME: " + firstName + " " +lastName +
                "\nCOMPANY EMAIL: " +email+
                "\nMAILBOX CAPACITY: " + mailBoxCapacity + "mb";
    }
}
