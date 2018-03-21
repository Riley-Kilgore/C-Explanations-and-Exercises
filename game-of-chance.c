/*
 The following is a game of chance wherein there are multiple options for the game a user would like to play.
 There is also the opportunity to reset a users number of tokens. Each user on a unix like system will have
 a different 'save' if you will. The user can create a screen name and bet against the program.

 This game compiles most of the information that I currently have when it comes to C programming, the game
 makes use of reading and writing files, as well as data segmentation, structs, and more.
*/
#include <stdio.h>
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
    printf("4 - View your highscore");
    printf("5 - Change your user name");
    printf("6 - Reset your credits at 100.");
    printf("7 - Quit the game\n");
    printf("Name: %s", player.name);
    printf("Credits: %u", player.credits);
    scanf("%d", choice);

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

}
