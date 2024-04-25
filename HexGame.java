/**
 * @author  Yahia Elsaad
 * Base Wars is a game that tests your knowledge on base converions. When you type play, you will be asked
 * to select a difficulty level. Then you will be asked base conversion questions based on that difficulty level.
 */

 import java.util.*;
 import java.io.BufferedReader;
 import java.io.BufferedWriter;
 import java.io.File;
 import java.io.FileReader;
 import java.io.FileWriter;
 import java.io.IOException;
 
 public class HexGame {
 
     private static int playerScore, playerStrikes, baseFrom, baseTo;
     private static String tempName; //current highscore name
     private static String hsNames[] = new String[10];
     private static int highScores[] = new int[10];
 
 
     public static void main(String[] args){
 
         //MARK:Load Save Data
         try{
             File tmp = new File("save.txt");
             if (!tmp.exists()){
                 BufferedWriter writer = new BufferedWriter(new FileWriter("save.txt"));
                 writer.write("Anna\nLeon\nYahia\nHilary\nZen\nJoe\nMama\nZack\nMikey\nDaniel\n10\n9\n8\n7\n6\n5\n5\n5\n5\n3");
                 writer.close();
             }
 
 
             BufferedReader reader = new BufferedReader(new FileReader("save.txt"));
             String retrieve;
             for(int i = 0;i <10;i++){
                 retrieve = reader.readLine();
                 hsNames[i] = retrieve;
             }
             for(int i = 0;i <10;i++){
                 retrieve = reader.readLine();
                 highScores[i] = Integer.parseInt(retrieve);
             }
 
             System.out.println("Current High Score by " + hsNames[0] + " in Hard Mode is: " + highScores[0]);
 
             reader.close();
 
         }catch (IOException e){
             e.printStackTrace();
         }
 
 
         //MARK:Main Menu
         Scanner sc = new Scanner(System.in);
         System.out.println("Type Play to start, type Rules to learn the rules of the game, type Leaderboard to load the leaderboard, or type Exit to stop the program.");
 
         String gameMode = sc.nextLine();
         gameMode = gameMode.toLowerCase();
 
 
 
         switch(gameMode) {
             case "play":
                 System.out.println("Select difficulty by entering a number from 1-3:\n1: Easy\n2: Medium\n3: Hard\n"); //TODO: use try catch for wrong input
 
                 int gameDifficulty = sc.nextInt(); //only works after pressing enter from game mode.  Might be able to use .endswith to combine and skip
                 while(gameDifficulty != 1 && gameDifficulty != 2 && gameDifficulty != 3){
                     System.out.println("not within range, try again");
                     gameDifficulty = sc.nextInt();
                 }
                 gameStart(gameDifficulty);
                 break;
 
             case "rule","rules":
                 System.out.println("\nYou will be given a start base value, a end base value, and a number value. You have to convert the number value from the first base to the second base. If you are correct you will be givien a point, if you are wrong, you get a strike. Three strikes and it is game over. High Scores are only saved for difficulty 3: Hard.\n");
                 HexGame.main(args);//restart to a "main menu"
                 break;
 
             case "exit":
                 System.exit(0);
 
             case "leaderboard":
                 leaderBoard();
                 break;
 
             default:
                 System.out.println("\nUnknown Command\n");
                 HexGame.main(args);
 
         }
         sc.close();
     }
 
     private static void leaderBoard(){
         System.out.println("-----leaderboard-----\n");
         for(int i = 0; i < 10; i++){
             System.out.println(hsNames[i] + " at " + highScores[i] + " points");
         }
         System.out.println("");
         main(null);
         
     }
 
     //Mark:Begin Round
     private static void gameStart(int difficultyLvl){
         try{
             Random rand = new Random();
             String val;
             if (difficultyLvl == 1){
                 baseFrom = rand.nextInt(3); //random base is picked from 0-2
                 baseTo = rand.nextInt(3); //the base we are quizzing on
 
                 for (;baseFrom == baseTo;){ //if the base we are quizzing on is the same as the base of the number, change it so that they are different.
                     baseTo = rand.nextInt(3);
                 }
 
                 if (baseFrom == 0){
                     baseFrom = 2;
                 }else if (baseFrom == 1){
                     baseFrom = 10;
                 }else{
                     baseFrom = 16;
                 }
                 if (baseTo == 0){
                     baseTo = 2;
                 }else if (baseTo == 1){
                     baseTo = 10;
                 }else{
                     baseTo = 16;
                 }
 
                 val =  Integer.toString(Integer.parseInt(Integer.toString(rand.nextInt(5000)),10),baseFrom); //creates a number in base 10 that goes up to 5000, converts it to the random base, then converts it to a string
 
 
 
 
             }else if (difficultyLvl == 2){
                 baseFrom = rand.nextInt(5); //random base is picked from 0-4
                 baseTo = rand.nextInt(5); //the base we are quizzing on
 
                 for (;baseFrom == baseTo;){ //if the base we are quizzing on is the same as the base of the number, change it so that they are different.
                     baseTo = rand.nextInt(5);
                 }
 
                 if (baseFrom == 0){
                     baseFrom = 2;
                 }else if (baseFrom == 1){
                     baseFrom = 8;
                 }else if(baseFrom == 2){
                     baseFrom = 10;
                 }else if (baseFrom == 3){
                     baseFrom = 16;
                 } else{
                     baseFrom = 32;
                 }
 
                 if (baseTo == 0){
                     baseTo = 2;
                 }else if (baseTo == 1){
                     baseTo = 8;
                 }else if(baseTo == 2){
                     baseTo = 10;
                 }else if (baseTo == 3){
                     baseTo = 16;
                 } else{
                     baseTo = 32;
                 }
 
                 val =  Integer.toString(Integer.parseInt(Integer.toString(rand.nextInt(25000)),10),baseFrom); //creates a number in base 10 that goes up to 25000, converts it to the random base, then converts it to a string
 
             }else{
                 baseFrom = 2 + rand.nextInt(35); //random base is picked from 2-36
 
                 val =  Integer.toString(Integer.parseInt(Integer.toString(rand.nextInt(100000)),10),baseFrom); //creates a number in base 10 that goes up to 100000, converts it to the random base, then converts it to a string
 
                 baseTo = 2 + rand.nextInt(35); //the base we are quizzing on
 
                 for (;baseFrom == baseTo;){ //if the base we are quizzing on is the same as the base of the number, change it so that they are different.
                     baseTo = 2 + rand.nextInt(35);
                 }
             }
 
 
             System.out.println("\nValue:\n" + val + "\nIn base:\n" + baseFrom + "\nTo base:\n" + baseTo + "\n");
             Scanner sc = new Scanner(System.in);
             String userInput = sc.nextLine();
 
             String correctValue = Integer.toString(Integer.parseInt(val,baseFrom),baseTo);
 
             if(userInput.toLowerCase().equals(correctValue)){ // || userInput.toLowerCase().contains("cheat") ){ //TODO remove this secret cheat
                 System.out.println("\nsuccess\nCurrent Score is: " + ++playerScore);
             }
             else{
                 System.out.println("\nfailure");
                 System.out.print("\nAnswer: " + correctValue + "\nStrikes: " + ++playerStrikes + "\n");
             }
         } catch (NumberFormatException e) { //generated via VS Code auto-fill: line
             // TODO Auto-generated catch block
             e.printStackTrace();
         }
         //MARK:Round Concluded
         if (playerStrikes != 3){ //continue the game
             gameStart(difficultyLvl);
          }
         else{ //game over
             System.out.println("\nGame Over\nFinal Score: " + playerScore);
             if(playerScore >= highScores[9] && difficultyLvl == 3){
                 System.out.println("You're on the Leader Board!\nPlease type your name!");
 
                 Scanner sc = new Scanner(System.in);
                 String usrInput = sc.nextLine();
                 tempName = usrInput;
                 sc.close();
 
                 for(int i = 0;i<10;i++){ //checking every element to see if the names are the same
                     if(hsNames[i].equals(tempName)){ 
                         if (playerScore <= highScores[i]){
                             System.out.println("Name already exist higher on the leaderboard! :D");
                             System.exit(0);
                         }else{
                             if(i == 0){ //changes top score
                                 highScores[0]= playerScore;
                                 break;
                             }else{
                                 for(int j = 0; j < 10; j++){
                                     if(playerScore >= highScores[j]){
                                         for(int k = i;k > j;k--){ //shift all elements, removing the element with the name.
                                             highScores[k] = highScores[k-1];
                                             hsNames[k] = hsNames[k-1];
                                         }
                                         highScores[j] = playerScore;
                                         hsNames[j] = tempName;
                                         break;
                                     }
                                 }
                             }
 
                             break; //does this break it all?
                         }
                     }
                 }
                 for(int i = 0;i<10;i++){//checking every element to see if the names are different
                     if(hsNames[i].equals(tempName)){
                         break;
                     }
                     if(i == 9){ //no names are already on the list, meaning this is a unique name.
                         for(int j = 0; j < 10; j++){
                             if(playerScore >= highScores[j]){
                                 for(int k = i;k > j;k--){ //shift all elements, removing the 10th element as a result
                                     highScores[k] = highScores[k-1];
                                     hsNames[k] = hsNames[k-1];
                                 }
                                 highScores[j] = playerScore;
                                 hsNames[j] = tempName;
                                 break;
                             }
                         }
                     }
                 }
                 sc.close();
                 try{
                     BufferedWriter writer = new BufferedWriter(new FileWriter("save.txt")); //save high score
                     for(int i = 0;i<10;i++){
                         writer.write(hsNames[i] + "\n");
                     }
                     for(int i = 0;i<10;i++){
                         writer.write(highScores[i] + "\n");
                     }
                     writer.close();
 
                 }catch (IOException e){
                     e.printStackTrace();
                 }
             }
         }
     }
 
 }
