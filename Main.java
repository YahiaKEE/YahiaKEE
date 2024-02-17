// As of 2/13/2024, this is a U.S dollar converter programs that converts 10 currencies
// I have chosen to U.S Dollars
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Hey there! This is a Currency Converter.\nFirst enter what type of currency you have. [Country] [Name].\nExample: Jordanian Dinar");
        String input = sc.nextLine().toLowerCase();
        switch (input) {
            case "jordanian dinar":
                System.out.println("What amount of Jordanian Dinar do you want to convert?");
                double Dinar = sc.nextFloat();
                Converter.Dinar(Dinar);
                break;
            case "australian dollar":
                System.out.println("What amount of Australian Dollars would you like to convert?");
                double australian = sc.nextFloat();
                Converter.australian(australian);
                break;
            case "european euro":
                System.out.println("What amount of European Euro's would you like to convert?");
                double euro = sc.nextFloat();
                Converter.euro(euro);
                break;
            case "canadian dollar":
                System.out.println("What amount of Canadian Dollar's would you like to convert?");
                double canadian = sc.nextFloat();
                Converter.canadian(canadian);
                break;
            case "russian ruble":
                System.out.println("What amount of Russian Ruble's would you like to convert?");
                double ruble = sc.nextFloat();
                Converter.ruble(ruble);
                break;  
            case "japanese yen":
                System.out.println("What amount of Japanese Yen's would you like to convert?");
                double yen = sc.nextFloat();
                Converter.yen(yen);
                break;
            case "mexican peso":
                System.out.println("What amount of Mexican Peso's would you like to convert?");
                double peso = sc.nextFloat();
                Converter.peso(peso);
                break;
            case "chinese yuan":
                System.out.println("What amount of Chinese Yuan's would you like to convert?");
                double yuan = sc.nextFloat();
                Converter.yuan(yuan);
                break;
            case "hong kong dollar":
                System.out.println("What amount of Hong Kong Dollar's would you like to convert?");
                double hongkong = sc.nextFloat();
                Converter.hongkong(hongkong);
                break;
            case "saudi riyal":
                System.out.println("What amount of Saudi Riyal's would you like to convert?");
                double riyal = sc.nextFloat();
                Converter.riyal(riyal);
                break;
            default:
                System.out.println("Please make sure you are typing the [Country] [Currency Name] \nEx: Australian Dollar");
                break;
        }
        sc.close();
    }
}