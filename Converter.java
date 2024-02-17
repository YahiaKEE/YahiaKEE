public class Converter {
    public static int Dinar(double Dinar) {
        double DinarResult = (Dinar * 0.71);
        String dinarformatted = String.format("%.2f", DinarResult);
        System.out.println("You have "+dinarformatted+" U.S Dollars");
        return 0;
    }
    public static int australian(double australian) {
        double australianResult = (australian / 1.53);
        String australianformatted = String.format("%.2f", australianResult);
        System.out.println("You have "+australianformatted+" U.S Dollars");
        return 0;
    }
    public static int euro(double euro) {
        double euroResult = (euro / 0.93);
        String euroformatted = String.format("%.2f", euroResult);
        System.out.println("You have "+euroformatted+" U.S Dollars");
        return 0;
    }
    public static int canadian(double canadian) {
        double canadianResult = (canadian / 1.36);
        String canadianformatted = String.format("%.2f", canadianResult);
        System.out.println("You have "+canadianformatted+" U.S Dollars"); 
        return 0;
    }
    public static int ruble(double ruble) {
        double rubleResult = (ruble / 91.39);
        String rubleformatted = String.format("%.2f", rubleResult);
        System.out.println("You have "+rubleformatted+" U.S Dollars");
        return 0;
    }
    public static int yen(double yen) {
        double yenResult = (yen / 150.72);
        String yenformatted = String.format("%.2f", yenResult);
        System.out.println("You have "+yenformatted+" U.S Dollars");
        return 0;
    }
    public static int yuan(double yuan) {
        double yuanResult = (yuan / 7.19);
        String yuanformatted = String.format("%.2f", yuanResult);
        System.out.println("You have "+yuanformatted+" U.S Dollars");
        return 0;
    }
    public static int hongkong(double hongkong) {
        double honkongResult = (hongkong / 7.82);
        String hongkongformatted = String.format("%.2f", honkongResult);
        System.out.println("You have"+hongkongformatted+" U.S Dollars");
        return 0;
    }
    public static int riyal(double riyal) {
        double riyalResult = (riyal / 3.75);
        String riyalFormatted = String.format("%.2f", riyalResult);
        System.out.println("You have "+riyalFormatted+" U.S Dollars");
        return 0;
    }
    public static int peso(double peso) {
        double pesoResult = (peso / 17.20);
        String pesoFormatted = String.format("%.2f", pesoResult);
        System.out.println("You have "+pesoFormatted+" U.S Dollars");
        return 0;
    }
}
