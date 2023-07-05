/*
# --------------------------------------------------------------------------------------------------------------------
#   Client Server Processes
#
#   Name: client_fifo.c 
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: This program acts as the client that will communicate with the server_fifo.c
#   program. The programs use Fifo to communicate with each other. The server_fifo.c code
#   is run in another teminal first, folllowed by running the client code in another terminal, 
#   by sending a filepath as the argument. The client when connects to server sends a message that is 
#   filename. Then it proceeds to open the file and copy it through the channel to the server 
#   using read() and write()
#   
#
#   usage: gcc client_fifo.c -o client
#          eg: ./client diplodocus.pdf
#              
#
#   Description of Parameters:
#       argv[1] - File path to the file name to be read and written
#
#   Subroutines/libraries required: 
#       See include statements
#           stdio.h
#           stdlib.h
#           netinet/in.h
#           arpa/inet.h
#           netdb.h
#           sys/socket.h
#           string.h
#           unistd.h
#           fcntl.h
# --------------------------------------------------------------------------------------------------------------------
*/

// include libraries 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

// max block size to be read from file
#define BUFSIZE 1024
// name of the fifo
#define FIFO_NAME "server_fifo"

// struct to store file contents being read from client and written in the new file
typedef struct message_pkg{
    int nbytes;
    char payload[BUFSIZE];
}message_pkg;

// main
int main(int argc, char *argv[]){
    
    if(argc<2){
        printf("Usage: send-msg msg...\n");
        exit(1);
    }

    // creating a char array of same size as all message to store file name
    char filename[BUFSIZE]; 
    //get the filename
    strncpy(filename,argv[1],strlen(argv[1])+1);

    // Pointer to the file
    char *file = argv[1];
    // store file descriptor, server file descriptor and number of bytes read from the file
    int fd, server_fd;
    //struct to store the file information 
    message_pkg content;
    

    // Open the file for reading
    if((fd = open(file, O_RDONLY)) == -1){
        perror("open file");
        return 1;
    }

    // Open the FIFO channel named server_fifo. Check if it exists or not then exit. 
    if(access(FIFO_NAME, F_OK) == -1){
        perror("access FIFO_NAME");
        exit(1);
    }

    // create a server file descriptor for fifo
    if((server_fd = open(FIFO_NAME, O_RDWR)) == -1){
        perror("open FIFO_NAME");
        exit(1);
    }

    // Send filename to server
    if (write(server_fd, filename, BUFSIZE) == -1) {
        perror("write filename");
        exit(1);
    }


    // Send file contents to server as a struct after reading
    while ((content.nbytes = read(fd, content.payload, BUFSIZE)) > 0) {
        // writing into server file descriptor fifo
        if (write(server_fd, &content, sizeof(content)) == -1) {
            perror("write");
            exit(1);
        }
    }
    // close the file
    close(fd);

    // if there was error reading from file
    if (content.nbytes == -1) {
        perror("read file error");
        exit(1);
    }

    // confirmation message that sent everything
    printf("File transfer complete.\n");

    // close the fifo file descriptor
    close(server_fd);
    return 0;
} 
