/*
 The following is a game of chance wherein there are multiple options for the game a user would like to play.
 There is also the opportunity to reset a users number of tokens. Each user on a unix like system will have
 a different 'save' if you will. The user can create a screen name and bet against the program.

 This game compiles most of the information that I currently have when it comes to C programming, the game
 makes use of reading and writing files, as well as data segmentation, structs, and more.
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include "couple.h"

#define DATAFILE "/var/chance.data" // File that users' game data is stored.

struct user{
  int uid;
  int credits;
  int highScore;
  char name[100];
  int (*currentGame)();
};

//Function prototypes for the game.
int getPlayerData();
void registerNewPlayer();
void updatePlayer();
void showHighScore();
void jackpot();
void inputNewName();
void printCards(char *, char *, int);
int takeWager(int, int);
void playGame();
int pickNumber();
int dealerNoMatch();
int findTheAce();
void fatal(char*);

// In order to avoid passing this variable around the entirety of the program, we variable globalizing it.
struct user player;

int main(){
  int choice, lastGame;

  srand(time(0));

  if(getPlayerData() == -1) // Then there probably isn't data for the user.
    registerNewPlayer();

// Display the menu to the player.
  while(choice != 7){
    printf("=========Menu=========");
    printf("1 - Play Pick a Number");
    printf("2 - Play the Match Dealer Game");
    printf("3 - Play Find the Ace");
    printf("4 - View your high score");
    printf("5 - Change your user name");
    printf("6 - Reset your credits at 100.");
    printf("7 - Quit the game\n");
    printf("Name: %s", player.name);
    printf("Credits: %u", player.credits);
    scanf("%d", &choice);

    if((choice < 1) || (choice > 7)){ // They didn't read the instructions or they're dying of boredom. (Been there).
      printf("That wasn't a choice available to you.");
    }else if(choice < 4){ // They want to play a game.
      if(choice != lastGame){
        switch(choice){
          case 1: player.currentGame = pickNumber; break;
          case 2: player.currentGame = dealerNoMatch; break;
          case 3: player.currentGame = findTheAce; break;
        }
        playGame();
      }
    }else if(choice == 4){
      showHighScore();
    }else if(choice == 5){
      printf("Change username:\n");
      printf("Enter a new user name: ");
      inputNewName();
      printf("\n Your name is changed! \n");
    }else if(choice == 6){
      printf("Your credits have been reset to 100!");
      player.credits = 100;
    }
  }
  updatePlayer();
  printf("Hope you come again soon!");
}

// This following function reads data from the file on a given user.
int getPlayerData(){
  int fd, uid, readBytes;
  struct user entry;

  uid = getuid(); // Grabs the user id of the person starting the game.

  fd = open(DATAFILE, O_RDONLY);

  if(fd == -1) // The file didn't open, we may need to make one.
    return -1;
  readBytes = read(fd, &entry, sizeof(struct user)); // Reading the data in chunks.
  while(entry.uid != uid && readBytes > 0){
    readBytes = read(fd, &entry, sizeof(struct user)); /* Continues to read bytes in chunks the size of the user struct
                                                        until the file is either empty or the correct user is found. */
  }
  close(fd);
  if(readBytes < sizeof(struct user)) // Then EOF was reached and the user is not yet in the file.
    return -1;
  else
    player = entry;
  return 1; // Code ran successfully if we get here and set the player.
}

// This is the function for a new user to register for our game.
// It will add their information to the file where information is saved.
void registerNewPlayer(){
  int fd;

  printf("Enter your name: ");
  inputNewName();

  player.uid = getuid();
  player.highScore = player.credits = 100;

  fd = open(DATAFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
  if(fd == -1)
    fatal("in registerNewPlayer() while opening file.");
  write(fd, &player, sizeof(struct user));
  close(fd);

  printf("\n Welcome to the Game of Chance %s.\n", player.name);
  printf("You have been given %u credits.\n", player.credits);
}

// Function to display the user's current high score.
void showHighScore(){
  unsigned int topScore = 0;
  char topName[100];
  struct user entry;
  int fd;

  printf("\n=========================|High Score|===========================\n");
  fd = open(DATAFILE, O_RDONLY);
  if(fd == -1)
    fatal("in showHighScore() while opening file.");
  while(read(fd, &entry, sizeof(struct user)) > 0){ // We continue to loop until EOF
    if(entry.highScore > topScore){
      topScore = entry.highScore;
      strcpy(topName, entry.name);
    }
  }
  close(fd);
  if(topScore > player.highScore)
    printf("You currently have the high score of all users with a total of %u credits.", player.highScore);
  printf("\n=============================================================================\n\n");
}

// This is a function to award a jackpot to the player.
void jackpot(){
  printf("============== You won the jackpot!! ===============");
  player.credits += 100;
}

//This is the function a player uses to change and create their name.
void inputNewName(){
  char *namePtr, inputChar = '\n';
  while(inputChar == '\n'){ // Eat those new lines.
    scanf("%c", &inputChar);
  }
  namePtr = (char *) &player.name;
  while(inputChar != '\n'){
    *namePtr = inputChar;
    scanf("%c", &inputChar);
    namePtr++;
  }
  *namePtr = 0;
}

// This function is for the find the ace game, it prints out the cards.
void printCards(char *message, char *cards, int userPick){
  int count;

  printf("\n\t*** %s ***\n", message);
  printf("     \t._.\t._.\t._.\n");
  printf("Cards:\t|%c|\t|%c|\t|%c|\n\t", cards[0], cards[1], cards[2]);
  if(userPick == -1){
    printf(" 1 \t 2 \t 3\n");
  }else{
    for(count = 0; count < userPick; count++){
      printf("\t");
    }
    printf(" ^-- your pick\n");
  }
}

// This function takes wagers from the user.
int takeWager(int availableCredits, int previousWager){
  int wager, totalWager;

  printf("How many of your %d credits would you like to wager? ", availableCredits);
  scanf("%d", &wager);
  if(wager < 1){
    printf("You can't bet less than a single credit...");
    return -1;
  }
  totalWager = previousWager + wager;
  if(totalWager > availableCredits){
    printf("You can't bet more credits than you have available to you.");
    return -1;
  }
  return wager;
}

// This function contains a loop that allows the current game to be replayed.
void playGame(){
  int playAgain;
  int (*game)();
  char selection;

  while(playAgain != 0){ // So long as the player wants to play again.
    if(player.currentGame() != -1){ // If the game threw an error.
      if(player.credits > player.highScore)
        player.highScore = player.credits;
      printf("You currently have %u credits.\n", player.credits);
      updatePlayer();
      printf("Would you like to play again? (y/n) ");
      selection = '\n';
      while(selection == '\n'){
        scanf("%c", &selection);
      }
      if(selection == 'n')
        playAgain = 0;
    }else{
      playAgain = 0; // Here the player would have experienced an error and so
                     // they cannot play again.
    }
  }
}

// This is the pick a number game.
int pickNumber(){
  int pick, winningNum;

  printf("\n####### Pick a Number #######\n");
  printf("This game costs 10 credits to play. Just pick a number\n between 1 and 20, if you pick the right number, you win 100 credits!");
  winningNum = (rand() % 20) + 1;
  if(player.credits < 10){
    printf("You only have %c credits, you can reset your credits at the main menu.", player.credits);
    return -1;
  }
  player.credits -= 10;
  printf("10 credits have been deducted from your account.\n");
  printf("Pick a number between 1 and 20.");
  scanf("%d", &pick);

  printf("The correct number was %d", winningNum);
  if(pick == winningNum){
    jackpot();
  }else{
    printf("Sorry! You lose..");
  }
  return 0;
}

// This is the no match dealer game.
int dealerNoMatch(){
  int i, j, numbers[16], wager = -1, match = -1;

  printf("\n####### No Match Dealer #######\n");
  printf("In this game the player can wage as many of their credits as desired.");
  printf("You will be dealt 16 random numbers between 0 and 99, then if there are no matches you win and double your wager.");

  if(player.credits == 0){
    printf("You don't have any credits to wager.");
    return -1;
  }
  while(wager == -1)
    wager = takeWager(player.credits, 0);

  printf("::Dealing Numbers::");

  for(i = 0; i < 16; i++){
    numbers[i] = rand()%100;
    printf("%2d\t", numbers[i]);
    if(i%8 == 7)
      printf("\n");
  }
  for(i=0; i < 15; i++){
    j = i +1;
    while(j < 16){
      if(numbers[i] == numbers[j])
        match = numbers[i];
      j++;
    }
  }
  if(match != -1){
    printf("The number %d got matched, so you lose.. I'm taking those credits now.", match);
    printf("%d credits have been removed from your account.", wager);
    player.credits -= wager;
  }else{
    printf("Looks like you won! I'll add those credits to your account.");
    printf("%d credits have been added to your account.", wager);
    player.credits += wager;
  }
  return 0;
}

// The following function is the find the ace game.
int findTheAce(){
  int i, ace, wager;
  int invalid, pick = -1, wager1 = -1, wager2 = -1;
  char choiceTwo, cards[3] = {'X', 'X', 'X'};

  ace = rand()%3;

  printf("####### Find the Ace #######\n");
  printf("In this game you can wager up to all of your credits.\n");
  printf("If you correctly guess the ace's position you win, otherwise you lose your credits.\n");
  printf("In the first round, one queen is revealed, at this point you can change your choice.\n");
  printf("In the second round, the ace is revealed and the game is over.");

  if(player.credits == 0){
    printf("You don't have any credits to wager..");
    return -1;
  }

  while(wager1 == -1){
    wager1 = takeWager(player.credits, 0);
  }

  printCards("Dealing Cards", cards, -1);
  pick = -1;
  while(pick < 1 || pick > 3){
    printf("Pick a card, 1, 2, or 3.");
    scanf("%d", &pick);
  }
}
