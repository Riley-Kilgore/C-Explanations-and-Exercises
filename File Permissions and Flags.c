/*
Created on 03/16/2018 by Riley Kilgore for educational purposes.
File flags in C are used to denote permissions that one has for opening and
editing a given file as well as to denote the permissions that a given program has
to do the same.
In C the fcntl.h header contains binary flags that have to do with what a given program
can do, read, write, append, create, etc.
The sys/stat.h header on the other hand contains flags that have to do with permissions,
for example, when creating a new file through the use of a program, if the flags
S_IRUSR and S_IWUSR are provided, then the user will have read and write permissions to the
file created.
Lets do some examples.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

void flagDemo(){
  char * datafile = (char*) malloc(100);
  strcpy(datafile, "/tmp/path/goes/here");
  int fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR); // This line will create a file descriptor pointing to the file,
  // at the given file path for datafile. If there is not a file there then it will make one because it has the O_CREAT flag.
  // The third parameter to open states that if the file needs to be created, then the user will have read and write permission.
  // The use of | in the parameters will be explained later, (See line ).
  free(datafile);
}

int main(void){
  int i, bitA, bitB;
  printf("bitwise OR |\n");
  for(i=0; i<4; i++){
    bitA = (i & 2) / 2; //This line gives bitA the second bit of a binary number. So if i=2 then in binary this is 10 and the second bit is 1.
    bitB = (i & 1); // This line gives bitB the first bit. The & is the bitwise AND, which means that it will return the number iff both of the sides contain the bit.
    // So in the first example. 2 & 2 returns 2, or in binary terms, 10 & 10 returns 10, which is to say 3 & 2 would return 2, because 3 -> 11 and 2 -> 10.
    printf("%d | %d = %d\n", bitA, bitB, bitA | bitB); // The bitwise OR | says that if any of the given bits is 1 then the returning bit will be one.
    // Using our examples from before. 2 | 2 returns 2, but 3 | 2 returns 3, in binary terms 11 | 10 is 11.

    /*PLEASE NOTE THAT ALL CONVERSIONS TO BINARY ARE JUST CONVERSIONS AND THE BINARY REPRESENTATIONS OF THOSE DECIMAL NUMBERS IS NOT THEIR REPRESENTATION SHOWN*
    *******************************In other words, 3 => 11, but 11 does not go to 11. ****************************************************/
  }
  printf("bitwise AND &\n");
  for(i=0; i<4; i++){
    bitA = (i & 2) / 2;
    bitB = (i & 1);
    printf("%d & %d = %d\n", bitA, bitB, bitA & bitB);
  }
}
