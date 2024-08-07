package studentdatabaseapp;
import java.util.Scanner;

public class Student {
    private String firstName;
    private String lastName;
    private int year;
    private String studentID;
    private String courses = null;
    private int TuitionBalance = 0;
    private static int costOfCourse = 600;
    private static int id = 1000;
    //Constructor: Prompt user to enter student's name and year
    public Student() {

        Scanner in = new Scanner(System.in);
        System.out.println("Enter student's first name: ");
        this.firstName = in.nextLine();

        System.out.println("Enter student last name: ");
        this.lastName = in.nextLine();

        System.out.println("1. Freshman\n2. Sophomore\n3. Junior\n4. Senior\nEnter student's grade level status: ");
        this.year = in.nextInt();
        setStudentID();
        
    }
    // Generate an ID
    private void setStudentID() {
        // Grade level + ID
        id++;
        this.studentID = year + "" + id;
    }
    // Enroll in courses
    public void enroll() {
        // Get inside a loop, user hits 'q' when they're done enrolling
        do {
            System.out.println("Enter course to enroll ('Q' to quit)");
            Scanner in = new Scanner(System.in);
            String course = in.nextLine();
            if (!course.equals("Q")) {
                courses = courses + "\n  " + course;
                TuitionBalance = TuitionBalance + costOfCourse; 
            }
            else{ break; }
        } while(1 != 0);
            
       
    }
    // View balance and pay tuition
    public void viewBalance() {
        System.out.println("Your balance is now: "+TuitionBalance);
    }

    // Pay Tuition
    public void payTuition() {
        viewBalance();
        System.out.println("Enter your payment: ");
        Scanner in = new Scanner(System.in);
        int payment = in.nextInt();
        TuitionBalance = TuitionBalance - payment;
        System.out.println("Thank you for your payment of "+payment);
        viewBalance();
        
    }
    // Show status
    public String showInfo() {
        return "Name: " + firstName + " " + lastName +
                "Grade Level: " + year +
                "\nStudent ID: " +studentID +
                "\nCourses enrolled: " + courses +
                "\nBalance : "+TuitionBalance;
    }


}
