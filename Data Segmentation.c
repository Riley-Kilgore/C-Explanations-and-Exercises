/* Created on 03/13/2018 by Riley Kilgore for educational purposes.
 * There are 5 segments that information will be loaded with regard to a general program. They are as follows:
 * 1) The 'text segment' or code segment is the binary of the file being ran loaded into memory.
 * 2) The data segment is where all instantiated global and static variable values reside.
 * 3) The bss is where non-instantiated global and static variable placeholders reside.
 * The first 3 segments are static with regards to their size in memory, at compile time their sizes are determined.
 * 4) The stack, this is where all temporary variable values reside, the stack is a literal stack data structure just as one
 * may implement using code. It is First In Last Out, this will be explained further in the program.
 * 5) The heap, the heap contains data that is allocated using malloc() and will remain allocated on the heap until it is freed or
 * the program is terminated.
 * The 4th and 5th segments are dynamically allocated, that is their size in memory is not determined at compilation time, but instead they grow and shrink
 * in memory as the program is run.
 */

void function1(){
    static int bss; // This variable would be held on the bss segment, whereas
    static int data = 5; // this one would be held on the data segment.
    // The same thing occurs with global variables.
    int stack;
    int stack2 = 5; // Both of these variables are held on the stack in what is called the stack frame of the function.
    // They are held in memory at the top of the stack until the execution of this function is terminated at which point they
    // will be popped or removed from the top of the stack and the function wherein this function was called will be at the top of the stack.
    // If this function calls another function, then that function will have it's own stack frame and it will be pushed to the top of the stack.
}

int main(void){
    char * heap = (char *) malloc(50); // This allocated 50 bytes of memory for the pointer called heap, ironically enough, the
    // value of heap is stored in the heap, or maybe I set things up that way?? Either way, the heap is a dynamically allocated
    // data structure that is able to hold information and malloc() is the function called to allocate memory on the heap.
    // So as to avoid the heap and stack colliding with one another in memory, the heap grows downwards to higher and higher memory addresses,
    // whereas the stack grows upwards towards lower and lower memory addresses, when the two data structures collide a program is said to have
    // run out of memory, meaning that there are no longer and free pointers within the memory allocated to a program for the program to hold information in.

    function1(); // At this point, all of the variables within the program are in memory, the static variables are in bss and data, the stack contains information
    // for registers on the processor about both the main function as well as function1() as well as the two stack variables which are held on the stack within the
    // stack frame of function1()

    // Now the only thing that has changed is that the stack frame of function1() has been popped from the stack, so the only thing that the stack contains is
    // information about registers for the main function.

    free(heap); // The free function takes a pointer in the heap and frees that memory, this is one way to avoid a program from running out of memory.
    // At this point, the heap is empty, and the stack only contains the very minimal amount of information it can hold, this information is known as the function
    // prelude to the main function, each function has a prelude that is loaded in with the functions stack frame.
    // It should be noted that this entire time, the only two segments wherein data was changed in memory are those of the stack and heap, as I said before,
    // the code segment, the bss, and the data segment are all determined to contain a specified amount of memory at the time of compilation.
}


// If anyone finds this and finds it interesting, I would love to see a star on github, it would make my day, otherwise I'm going to try and continue posting info like this
// for my own educational purposes. Thanks for reading!!