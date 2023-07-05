
/*
# --------------------------------------------------------------------------------------
#   Command Line Arguments and File Processing
#
#   Name: 1_reverse_file.c, 
#
#   Written By: Punyaja Mishra - February 2023
#
#   Purpose: Read a file and copy its contents to another file in reverse order
#
#   usage: gcc 1_reverse_file.c -o reverse_file
#          eg: ./reverse_file /home/COIS/3380/lab2_document.txt /home/punyajamishra/3380/lab2/reversed_file1.txt 
#
#   Description of Parameters:
#       argv[1] - input file who content are read and copied in the reverse order
#       argv[2] - output file to which the content are copied
#
#   Subroutines/libraries required: 
#       See include statements
#       this routine uses the unistd.h for ..... 
#           and fcntl.h for ......
#
# --------------------------------------------------------------------------------------
*/

// include libraries 
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

//defining buffer size 
#define BUFSIZE 512

//copyfile method to open, read, write and close file
int copyfile(const char *name1, const char *name2)
{
    //to store the accepted method parameters of input and output file
    int infile, outfile;
    //store the number of bytes being read (for this program its 1)
    int nread;
    //buffer that will store the read content and to be copied content
    char buffer[BUFSIZE];
    //position of the pointer in the file
    int position;

    //open infile as READ ONLY
    //if can't open, then return error with int -1
    if((infile = open(name1, O_RDONLY)) == -1)
        return -1;
    
    //open outfile as WRITEONLY, CREATE AND TRUNCATE
    //if can't open then return error with int -2
    //0644 means file with user read write access (110 in binary = 6 in decimal), 
    //  group read access (100 bianry = 4), others read access (100 -> 4)
    //  first '0' is the file, not directory
    if((outfile = open(name2, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        close(infile);
        return -2;
    }

    //set the pointer to the end of the file
    position = lseek(infile,-1, SEEK_END);

    //now looping until we reach the start of the file, which means position < 0
    //reading the current character and writing into the file
    while(position >= 0)
    {
        //check something is read, that means we are not at the end of the file
        if((nread = read(infile, buffer, 1)) > 0)
        {
            //write the read content (buffer) and make sure all read content
            //  are copied ('nread' - read content)
            //if written content is less than the read content, then return error with -3
            //  and close both input and output files
            if(write(outfile, buffer, nread) < nread)
            {
                close(infile); close(outfile);
                return -3;
            }
            //make sure the pointer is set to the next "previous" position (reversed order)
            //  which would be -2 from the current position, because 
            //  after reading the current position is back to the end of the file
            position = lseek(infile, -2, SEEK_CUR);
        }
    }

    //close the input and output files
    close(infile); close(outfile);

    //if the 'read' bytes is -1 that means not read properly return with error -4
    if(nread == -1)
        return -4;
    //else everything went good, return 0
    else    
        return 0;
    
  
}

// main function that takes comman line parameters
int main(int argc, char *argv[])
{
    //there should be 3 arguments
    //less arguments or too many arguments - print error and exit with status 0
    if(argc != 3){
        printf("Error : Not right number of arguments provided");
        exit(0);
    }

    //store the first argument as input file
    char *inputFile = argv[1];
    //store the seconf argument as output file
    char *outputFile = argv[2];

    //define result that will store the return value of the method copyfile()
    int result;
    
    //check if input file exists
    if(access(inputFile,F_OK == 0))
    {
        //call copyfile method and test return code
        result = copyfile(inputFile, outputFile);

        //swithc case that prints the error based on the return value from the copyfile() method
        switch(result)
        {
            //everything was successful/ no errors
            case 0: printf("Contents copied from %s to %s\n", inputFile, outputFile);
                    break;
            //error in opening input file
            case -1: printf("Error opening input file: %s\n", inputFile);
                    break;
            //error in opening output file
            case -2: printf("Error opening output file: %s\n", outputFile);
                    break;
            //error in writing to output file
            case -3: printf("Error writing to file %s\n", inputFile);
                    break;
            //error while writing in last to output file
            case -4: printf("Error in last write to file %s\n", inputFile);
                    break;
            //some other unknown error
            default: printf("Unknown error occured %s\n", inputFile);
                    break;         
        }

        //if all good, then exit with status 1
        if(result = 0)
            exit(1);
        //else exit with status 0
        else
            exit(0);

    }
    else
    {
        printf("File to be copied does not exit. Please check your filepath");
        exit(0);
    }

    
}


