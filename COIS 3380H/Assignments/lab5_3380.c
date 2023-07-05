/*
# --------------------------------------------------------------------------------------------------------------------
#   Signal Processing in C
#
#   Name: signal.c 
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: Program contains 3 signal handlers functions. One handler handles ^C,
#           other creates a child process when user enters ^\ and then executes the ^\.
#           User is allowed to enter ^C 5 times after which the signal is deactivated and
#           ^C fucntion is executed. 
#           SIGINT            : ^C - cancels function when entered 6th time
#           SIGQUIT           : ^\ - creates a new child process that prints message 
#                               and uses kill() to call SIGUSR1 signal to 3rd handler
#           SIGUSR1           : Prints program is over and quits
#
#   usage: gcc signal.c -o signal_1
#          eg: ./signal_1
#               enter ^C - as many times as you want
#               enter ^\
#               
#
#   Description of Parameters:
#       No argv taken when calling program
#       Arguments(Signals) are taken as part of user porgram interaction
#
#   Subroutines/libraries required: 
#       See include statements
#       sys/types.h
#       sys/wait.h
#       unistd.h
#       stdio.h
# --------------------------------------------------------------------------------------------------------------------
*/

// include libraries 
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// Global variable that keeps count of number of times ^C was entered
int count = 0;
#define MAX_CTRL_C 5

/*
# --------------------------------------------------------------------------------------
#   Function: handler1(int signo)
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: Handles SIGINT signal (^C). 
#           Print message for 5 times then execute ^C to terminate program 
#
#   Description of Parameters:
#       int signo - stores the signal number
#
#   Subroutines/libraries required:
#       See include statements
#       unistd.h
#       stdio.h
#
# --------------------------------------------------------------------------------------
*/
static void handler1(int signo){
    // if ^C was entered less than 5 times, print a message with the counter (doing -1 because starting from 0)
    if(count<MAX_CTRL_C-1){
        // increment counter
        count++;
        printf("This is %d time you pressed ctrl-C (SIGINT)\n",count);
    }
    // ^C was entered 5th time - deactivating signal so next instance of ^C terminates program
    else{
        printf("Received ^C SIGNINT 5 times\n. Deactivting Signal");
        // deactivating signal
        signal(SIGINT, SIG_DFL);
    }
}

/*
# --------------------------------------------------------------------------------------
#   Function: handler2(int signo)
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: Handles SIGQUIT signal (^\). 
#           Creates child process. Child process prints a message and sends SIGUSR1 signal
#           using kill() to parent. Parent waits for child to finish
#
#   Description of Parameters:
#       int signo - stores the signal number
#
#   Subroutines/libraries required:
#       See include statements
#       unistd.h
#       stdio.h
#       sys/wait.h
#       sys/types/h
#
# --------------------------------------------------------------------------------------
*/
static void handler2(int signo){
    printf("Received ^\\ SIGQUIT and Creating CHILD PROCESS.\n");
    // creating child process
    pid_t pid = fork();
    // if in child process
    if(pid==0){
        printf("I am child process and I am sending a signal \n");
        // send SIGUSR1 to parent
        kill(getppid(),SIGUSR1);
        // exit without error code
        exit(0);
    }
    // parent waits for child
    else{
        wait((int *)0);
    }
}

/*
# --------------------------------------------------------------------------------------
#   Function: handler3(int signo)
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: Handles SIGUSR1 signal. 
#           Prints message that program is over
#
#   Description of Parameters:
#       int signo - stores the signal number
#
#   Subroutines/libraries required:
#       See include statements
#       unistd.h
#       stdio.h
#
# --------------------------------------------------------------------------------------
*/
static void handler3(int signo){
    // prints message
    printf("Received SIGUSER1 from child process, So looks like we are done. \n");
    printf("BTW, you pressed ^C %d times. \n",count);
    printf("Exiting Now... Bye Bye\n");
    // exiting
    exit(0);
}

// Main Function
int main(){
    int done = 0;

    printf("---- Let's Begin ----\n");

    // Calling functions for particualr signals. Printing error if Signal Error
    if( signal(SIGINT,handler1 ) == SIG_ERR ){
        printf("Cannot catch ^C SIGINT\n" );
    }

    // Calling functions for particualr signals. Printing error if Signal Error
    if( signal(SIGQUIT,handler2 ) == SIG_ERR){
        printf("Cannot catch ^\\ SIGQUIT\n");
    }

    // Calling functions for particualr signals. Printing error if Signal Error
    if(signal(SIGUSR1, handler3) == SIG_ERR){
        printf("Cannot catch SIGUSR1\n");
        exit(1);
    }

    // Looping forever waiting for signal
    while(!done){
        pause(); 
        /* pause until signal handler
        * has processed signal */
       printf("---- Waiting for another Signal ----\n");
    }

    return 0;
    
}