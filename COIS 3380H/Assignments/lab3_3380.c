/*
# --------------------------------------------------------------------------------------------------------------------
#   Directory, Files and Timestamps
#
#   Name: my_ls.c 
#
#   Written By: Punyaja Mishra - February 2023
#
#   Purpose: Read the user arguments as command line arguments (if entered) and 
#       perform the functions on the directory. If no directory path is entered by
#       the user, the perform functions on current directory. Following parmeters
#       are accepted:
#       -o int : print all files minimum entered integer days old
#       -y int : print all files maximum entered integer days old
#       -s int : print all files with size smaller than entered integer 
#       -l int : print all files with size greater than entered integer 
#
#   usage: gcc my_ls.c -o my_ls_1
#          eg: ./my_ls_1 /home/COIS/3380/lab3
#              ./my_ls_1 -o 250 /home/COIS/3380/lab3
#              ./my_ls_1 -y 3650 /home/COIS/3380/lab3
#              ./my_ls_1 -s 5000 /home/COIS/3380/lab3
#              ./my_ls_1 -l 10000 /home/COIS/3380/lab3
#              ./my_ls_1 -o 250 -y 3650 -s 5000 -l 1000 /home/COIS/3380/lab3
#              ./my_ls_1 -o 250
#              ./my_ls_1 -y 3650
#              ./my_ls_1 -s 5000
#              ./my_ls_1 -l 1000
#              ./my_ls_1 -o 250 -y 3650 -s 5000 -l 1000
#
#   Description of Parameters:
#       argv[argc-1] - Directory Path whose file to be read. If empty, then current directory is read
#       argv[i]      - command line parameter that specifies what command to execute (-o, -s ,-l, -y)
#       argv[i+1]    - integer value followed by the command line parameter
#
#   Subroutines/libraries required: 
#       See include statements
#       dirent.h
#       sys/types.h
#       sys/stat.h
#       time.h 
#       grp.h
#       pwd.h
#       errno.h
# --------------------------------------------------------------------------------------------------------------------
*/

// include libraries 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>


/*
# --------------------------------------------------------------------------------------
#   Function: is_integer(char *str)
#
#   Written By: Punyaja Mishra - February 2023
#
#   Purpose: Check if the argument is of type integer
#
#   Description of Parameters:
#       *str - the character to be checked if its an integer
#
#   Subroutines/libraries required:
#       See include statements
#       unistd.h
#       errno.h
#       string.h
#
# --------------------------------------------------------------------------------------
*/
int is_integer(char *str) {
    char *endptr;
    // conversion of string to long integer value
    long val = strtol(str, &endptr, 10);

    // Conversion failed, string is not a valid integer
    if (errno == ERANGE || *endptr != '\0' || endptr == str) {
        return 0;
    }
    // Conversion succeeded, string is a valid integer
    return 1;
}

/*
# --------------------------------------------------------------------------------------
#   Function: myDir(char *dir_Path, int min_size, int min_age, int max_size, int max_age)
#
#   Written By: Punyaja Mishra - February 2023
#
#   Purpose: Read the directory and print the expected file with the following information:
#       - iNode value
#       - Type of file
#       - Permissions
#       - UID
#       - GID
#       - Modification Date
#       - Size
#       - File Name
#
#   Description of Parameters:
#       *dir_Path - The Directory Path whose files are to be listed
#       min_size  - All files with size greater than min_size are printed
#       min_age   - All files created after min_age are printed 
#       max_size  - All files with size less than max_size are printed 
#       max_age   - All files created before max_age are printed 
#
#   Subroutines/libraries required:
#       See include statements
#       dirent.h
#       sys/types.h
#       sys.stat.h
#       time.h 
#       grp.h
#       pwd.h
#
# --------------------------------------------------------------------------------------
*/
int myDir(char *dir_Path, int min_size, int min_age, int max_size, int max_age)
{
    //Defining variables and pointers
    // directory to be opened and read and then closed at the end
    DIR *dp;
    // struct value to store details about the opnened directory
    struct dirent *dir;
    // struct value to store more details about the opened directory
    struct stat statbuf;
    // current time in seconds
    time_t mtime;
    // boolean flag to keep track if file fulfils all conditions
    int flag = 1;
    
    // Open directory 
    if((dp = opendir(dir_Path)) == NULL) 
    {
        //print error and exit if trouble opening directory
        fprintf(stderr, "Cannot open dir\n");
        exit(1);
    }

    // change current directory to the mentioned directory
    chdir(dir_Path); 

    //print headers of the table
    printf("inode\tType\tPermissions\tUID \t   GID\t\t   Modification date\t\tsize\t\tFilename\n");

    //loop through all contents of the directory being read as long as we don't reach end of directory
    while((dir = readdir(dp)) != NULL)
    {
        // flag = 0 means one or more conditionals were not met, and thus that file is not printed
        // boolean flag is set to 1 for each file being read
        flag = 1;
        // point the struct to current file being read to get its information
        stat(dir->d_name, &statbuf);

        // if all passed int parameters are less than 0 (= -1), then no extra parameters were passed
        // set flag to 1, to proceed to printing ALL files and is details
        if(min_size<0 && max_size<0 && min_age<0 && max_age<0)
        {
            flag=1;
        }

        // Check if "-s" parameter was entered and flag is not 0 
        if(max_size >=0 && flag!=0)
        {
            // check if file is less than the size mentioned
            if(statbuf.st_size < max_size){
                flag = 1;
            }
            // condition not met -> flag is set to 0 to not print the file
            else{
                flag = 0;
            }
        }
        
        // Check if "-l" parameter was entered and flag is not 0 
        if(min_size >=0 && flag!=0)
        {
            // check if file is greater than the size mentioned
            if(statbuf.st_size > min_size){
                flag = 1;
            }
            // condition not met -> flag is set to 0 to not print the file
            else{
                flag = 0;
            }
        }  

        // Check if "-y" parameter was entered and flag is not 0 
        if(min_age >=0 && flag!=0)
        {
            // check if file is newer than the age mentioned
            mtime = time(NULL);
            if(statbuf.st_mtime > (mtime-min_age)){
                flag = 1;
            }
            // condition not met -> flag is set to 0 to not print the file
            else{
                flag = 0;
            }
        }

        // Check if "-o" parameter was entered and flag is not 0 
        if(max_age >=0 && flag!=0)
        {
            mtime = time(NULL);
            // check if file is older than the age mentioned
            if(statbuf.st_mtime < (mtime-max_age)){
                flag = 1;  
            }
            // condition not met -> flag is set to 0 to not print the file 
            else{
                flag = 0;
            }    
        }
        

        // if our boolean, is not equal to 0, then the file passes all conditions and parameters
        // we print all the info about this file
        if(flag!=0)
        {   
            // print I-Node
            printf("%ld",(long) dir->d_ino);
            
            // print the Type of the file 
            if(S_ISREG(statbuf.st_mode)){ printf(" REG\t");} //Regular file
            if(S_ISDIR(statbuf.st_mode)){ printf(" DIR\t");} //directory file
            if(S_ISCHR(statbuf.st_mode)){ printf(" CHR\t");} //char special file
            if(S_ISBLK(statbuf.st_mode)){ printf(" BLK\t");} //block special file
            if(S_ISFIFO(statbuf.st_mode)){ printf(" PIPE\t");} //pipe or FIFO
            if(S_ISLNK(statbuf.st_mode)){ printf(" LINK\t");} //symbolic link 
            if(S_ISSOCK(statbuf.st_mode)){ printf(" SOCK\t");} //socket 

            // Print Permissions
            // user permissions
            if(statbuf.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if(statbuf.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if(statbuf.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            } 

            // group permissions 
            if(statbuf.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if(statbuf.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if(statbuf.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }

            // other permissions 
            if(statbuf.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if(statbuf.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if(statbuf.st_mode & S_IXOTH)
            {
                printf("x.  ");
            }
            else
            {
                printf("-.  ");
            }

            // Print UID
            printf(" %s\t", getpwuid(statbuf.st_uid)->pw_name);

            // Print GID
            printf(" %s\t", getgrgid(statbuf.st_gid)->gr_name);

            // Print Modification date
            char *mod_time = ctime(&statbuf.st_mtime);
            // Removing new line character at the end of the modification date
            mod_time[strcspn(mod_time, "\n")] = '\0'; 
            printf("%s\t", mod_time);

            // Print File Size
            printf("%d\t\t", statbuf.st_size);

            //Print FileName
            printf("%s\n", dir->d_name);
        }
    
    }
   
    // Closing the directory
    closedir(dp);
    // Return 0 - all good
    return 0; 
} 

// Main Function to read all entered arguments and call necessary functions
int main(int argc, char *argv[])
{
    // set default values of the parameters/conditions to be checked on the file in myDir()
    int min_age = -1;
    int max_age = -1;
    int min_size = -1;
    int max_size = -1;
    int i;

    //if only one argument is passed then it will assume the target directory is the one user is running
    // call myDir() function on current directory
    if(argc == 1)
        myDir(".", min_size, min_age, max_size, max_age);
    else
    {
        // Go through all the arguments entered by the user
        // We do not read the first argument as that is the executable file, therefore start from i=1
        for(i=1; i < argc; i++){
            
            // check if we are on the last argument
            // this is either the directory path or the integer for the previous parameter passed as argument
            if(i==(argc-1))
            {
                // check if the last value is NOT integer that means it is the filepath
                if(is_integer(argv[argc-1]) == 0)
                {
                    myDir(argv[argc-1], min_size, min_age, max_size, max_age);
                }
                // else no directory was mentioend, assume current directory
                else
                {
                    myDir(".", min_size, min_age, max_size, max_age);
                }
            }
            // we are not on last argument
            // make sure its not integer, because  the integer is for the previous parameter passed as argument
            else if(is_integer(argv[i]) == 0){
                // check if -o was passed
                if(strcmp(argv[i],"-o") == 0){
                    // take the next argument for value of max_age
                    max_age = atoi(argv[i+1]);
                    max_age = max_age * 84600;
                }
                // check if -s was passed
                else if(strcmp(argv[i],"-s") == 0){
                    // take the next argument for value of max_size
                    max_size = atoi(argv[i+1]);
                }
                // check if -l was passed
                else if(strcmp(argv[i],"-l") == 0){
                    // take the next argument for value of min_size
                    min_size = atoi(argv[i+1]);
                }
                // check if -y was passed
                else if(strcmp(argv[i],"-y") == 0){
                    // take the next argument for value of min_age
                    min_age = atoi(argv[i+1]);
                    min_age = min_age * 84600;
                }
                // none of the above accepted parameters were passed. Print error and exit
                else {
                    printf("Wrong argument entered. Please enter -o, -s, -l or -y");
                    exit(0);
                }
            }
            // if argument is an integer, make sure it is a positive number
            else{
                if(argv[i] < 0){
                    // if not, then print error and exit
                    printf("Please enter positive number");
                    exit(0);
                }
                
            }
        }
    }
    return 0;
}