package studentdatabaseapp;
import java.util.Scanner;

public class StudentDatabaseApp {

    public static void main(String[] args) {
        // Ask how many new students we want to add
        System.out.println("Enter number of new students to enroll: ");
        Scanner in = new Scanner(System.in);
        int numStudents = in.nextInt();
        Student[] students = new Student[numStudents];

        //Create 'x' number of new students
        for(int i = 0;  i < numStudents; i++) {
            students[i] = new Student();
            students[i].enroll();
            students[i].payTuition();
            System.out.println(students[i].showInfo());         
        
        }
        
    }

}