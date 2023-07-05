/*
# --------------------------------------------------------------------------------------------------------------------
#   Fork and Execute in C
#
#   Name: fork_exec.c 
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: Creating a new shell that accepts fixed verbs as user inputs and performs 
#       them in replacement of bash commands. The porgram recognize the verb requested 
#       and create a process to run the equivalent bash command:
#       help                :   lists all the available verbs
#       diskus              :   du -sch
#       listlg              :   ls -l -i on current directory
#       makedr[dir_name]    :   mkdir "dir_name", where dir_name is asked from the user
#       findwd              :   grep -n "twinkle" /home/COIS/3380/lab2_document.txt
#       logout              :   exit the program/ shell, return to original shell
#
#   usage: gcc fork_exec.c -o fork_exec_1
#          eg: ./my_ls_1
#               enter the verbs - help, diskus, listlg, findwd
#               enter makedr, then enter the name of the new directory
#
#   Description of Parameters:
#       No argv taken when calling program
#       Arguments are taken as part of user porgram interaction
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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
# --------------------------------------------------------------------------------------
#   Function: func_diskus()
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: Create a child process to execute du -sch while parent process waits
#
#   Description of Parameters:
#       None
#
#   Subroutines/libraries required:
#       See include statements
#       unistd.h
#       sys/wait.h
#
# --------------------------------------------------------------------------------------
*/
int func_diskus(){
    // create process using fork() and store the pid 
    pid_t pid = fork();
    
    // if child process
    if(pid==0){
        // execute du -sch
        execlp("du", "du", "-s", "-c", "-h",(char *)0);
        // if execlp() failed print and return '2'
        printf("exec() failed for diskus");
        return 2;
    }

    // if parent process
    else if(pid > 0){
        // wait and return 0
        wait((int *)0);
        return 0;
    }
    else{
        // return 1 if fork was unsuccessful 
        printf("Fork unsuccessful for diskus");
        return 1;
    }
}   

/*
# --------------------------------------------------------------------------------------
#   Function: func_listlg()
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: Create a child process to execute ls -l -i on current directory 
#           while parent process waits
#
#   Description of Parameters:
#       None
#
#   Subroutines/libraries required:
#       See include statements
#       unistd.h
#       sys/wait.h
#
# --------------------------------------------------------------------------------------
*/
int func_listlg(){
    // create process using fork() and store pid
    pid_t pid = fork();

    // if child process
    if(pid==0){
        // execute ls -l -i
        execlp("ls", "ls", "-l", "-i",(char *)0);
        // print if exec fails ad return 2
        printf("exec() failed for listlg");
        return 2;
    }

    // if parent process
    else if(pid > 0){
        //wait and return 0 if successful
        wait((int *)0);
        return 0;
    }
    else{
        // return 1 if fork unsuccessful 
        printf("Fork unsuccessful for listlg");
        return 1;
    }
}

/*
# --------------------------------------------------------------------------------------
#   Function: func_makedr(char dir_name[])
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: Create a child process to execute mkdir command with name "dir_name" 
#           while parent process waits
#
#   Description of Parameters:
#       None
#
#   Subroutines/libraries required:
#       See include statements
#       unistd.h
#       sys/wait.h
#
# --------------------------------------------------------------------------------------
*/
int func_makedr(char dir_name[]){
    // create array of commands
    char *args[]={"mkdir",dir_name,NULL};   
    // create process using fork() and store pid
    pid_t pid = fork();

    // if child process
    if(pid==0){
        // execute the commands
        execv("/usr/bin/mkdir",args);
        //if exec fialed then return 2
        printf("exec() failed for makedr");
        return 2;
    }

    // if parent process
    else if(pid > 0){
        //wait and return 0 if sccessful
        wait((int *)0);
        return 0;
    }
    else{
        // return 1 if fork unsuccessful
        printf("Fork unsuccessful for makedr");
        return 1;
    }
}

/*
# --------------------------------------------------------------------------------------
#   Function: func_findwd()
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: Create a child process to execute grep -n "twinkle" /home/COIS/3380/lab2/document.txt 
#           while parent process waits
#
#   Description of Parameters:
#       None
#
#   Subroutines/libraries required:
#       See include statements
#       unistd.h
#       sys/wait.h
#
# --------------------------------------------------------------------------------------
*/
int func_findwd(){
    // create array of commands
    char *arr[] = {"grep","-n","twinkle","/home/COIS/3380/lab2_document.txt",NULL};
    // create process using fork() and store pid
    pid_t pid = fork();

    // if child process
    if(pid==0){
        // execute command
        execv("/usr/bin/grep",arr);
        // if exec fails return 2
        printf("exec() failed for findwd");
        return 2;
    }

    // if parent process
    else if(pid > 0){
        //wait and return 0 if successful
        wait((int *)0);
        return 0;
    }
    else{
        // return 1 if fork unsuccessful
        printf("Fork unsuccessful for findwd");
        return 1;
    }
}

// Main Function 
int main(int argc, char *argv[]){
    
    // flag t
    int done = 0;
    // to store the return value to check if errors or fork and exec were successful
    int r_value;
    // to store the user input for verbs/commands
    char user_command[6];
    // store directory name for make directory command
    char dir_name[10];
    
    printf(" ------------ Welcome to my Shell ------------ \n");

    // while our flag is not set to 1 continue
    while(done!=1){
        // ask user to enter the command and store it in variable user_command
        printf("\nType 'help' to get the list of commands OR Enter a command from the help list: \n");
        scanf("%s",user_command);
        printf("\n");


        // if verb = "help" then list all list of available verbs
        if(strncmp(user_command,"help",4)==0){
            // help
            printf(" ---- Help ---- \n");
            // du -sch
            printf("diskus \t\t\t\t shows disk usage \n"); 
            // ls -li
            printf("listlg \t\t\t\t lists all files in the current directory with details \n");
            // mkdir
            printf("makedr [dir_name]  \t\t make new directory in current directory\n");
            // grep -n "Twinkle" /home/COIS/3380/lab2_document.txt
            printf("findwd \t\t\t\t finds word 'Twinkle' in /home/COIS/3380/lab2_document.txt \n");
            // exit()
            printf("logout \t\t\t\t exits shell and returns to original shell \n");
        }

        // if verb = "diskus" then call the function to perform disk use command
        else if(strncmp(user_command,"diskus",5)==0){
            // call function to fork and create process and run the command
            r_value = func_diskus();
        }

        // if verb = "listlg" then call the function to perform list command
        else if(strncmp(user_command,"listlg",5)==0){
            // call function to fork and create process
            r_value = func_listlg();
        }

        // if verb = "makedr" then call the function to perform mkdir command
        else if(strncmp(user_command,"makedr",5)==0){
            //ask user for name of new directory 
            printf("Make sure directory name is less than 10 characters please.\n");
            printf("Enter a name for the new directory:\n");
            scanf("%s",dir_name);
            printf("\n");

            // call function to fork and create process
            r_value = func_makedr(dir_name);
        }

        // if verb = "findwd" then call the function to perform find word command
        else if(strncmp(user_command,"findwd",5)==0){
            // call function to fork and create process
            r_value = func_findwd();
        }

        // if verb = "logour" then exit from the loop
        else if(strncmp(user_command,"logout",5)==0){
            // set flag to 1
            done = 1;
        }

        // any other input by user then print error
        else{
            printf("Please enter correct command available in my shell. \n");
        }

        // Exit if errors after printing the errors
        switch(r_value)
        {
            case 1:
                printf("Exited with Fork Error\n");
                exit(1);
            case 2:
                printf("Exited with Exec Error\n");
                exit(1);
        }
        
    }

    // Program finished, and user types "logout"
    printf("\nThanks for using My Shell. Bye Bye Now\n");
    printf("\n ------ Going back to Original Bash Shell -----\n\n");

    return 0;

}

