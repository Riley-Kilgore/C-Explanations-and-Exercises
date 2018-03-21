// A couple of useful functions.

// A function that displays an error message and exits the program.
void fatal(char * message){
  char errorMessage[100];

  strcpy(errorMessage, "[!!] Fatal Error ");
  strncat(errorMessage, message, 83);
  perror(errorMessage);
  exit(-1);
}

// An error-checked version of malloc
void * ecMalloc(unsigned int size){
  void * ptr;
  ptr = malloc(size);
  if(ptr == NULL){
    fatal("in ecMalloc for memory allocation.");
  }
  return ptr;
}
