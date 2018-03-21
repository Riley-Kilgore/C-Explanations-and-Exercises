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
      printf("That wasn't a choice available to you.")
    }else if(choice < 4){ // They want to play a game.
      
    }
  }
}
