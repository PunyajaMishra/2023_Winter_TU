/*
# --------------------------------------------------------------------------------------------------------------------
#   Client Server Processes
#
#   Name: server_fifo.c 
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: This program acts as the server that will communicate with the client.c
#   program. The programs use Fifo to communicate with each other. The program will
#   run in the background first, folllowed by execution of the client code, by sending 
#   a filepath as the argument. The server reads the filename and creates a new file
#   with "_server" at the end. The it keeps reading the file in blocks of 1024 and stops
#   when its less than 1024 because it means we are at the end of the file
#   
#
#   usage: gcc server_fifo.c -o server
#          eg: ./server 
#              
#
#   Description of Parameters:
#       None
#
#   Subroutines/libraries required: 
#       See include statements
#           stdio.h
#           stdlib.h
#           ctype.h
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

// max block size to be written in file
#define BUFSIZE 1024
// size of the string being added to the end of the filename
#define STR_LEN 7
// name of the fifo
#define FIFO_NAME "server_fifo"

// struct to store file contents being read from client and written in the new file
typedef struct message_pkg{
    int nbytes;
    char payload[BUFSIZE];
}message_pkg;

// main
int main(int argc, char *argv[]){

    // receive filename
    char filename[BUFSIZE];
    // define char array with string to the end of the filename
    char _server[STR_LEN] = "_server";
    
    // define filedescriptors and integers for number of bytes being read, and total bytes read
    int fd, client_fd, bytes_received, total_bytes_received = 0;
    // struct variable that is sent as package
    message_pkg content;

    // Create FIFO (named pipe) for client to connect to
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }

    // Open FIFO for reading and writing
    if ((client_fd = open(FIFO_NAME, O_RDWR)) == -1) {
        perror("open client fd");
        exit(1);
    }

    // Wait for client to send filename
    if ((bytes_received = read(client_fd, filename, BUFSIZE)) == -1) {
        perror("read filename");
        exit(1);
    }

    // concatenate "_server" at the end of filename
    strncat(filename,_server,strlen(_server));

    // create a new file with the filename 
    if ((fd = open(filename,  O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1) {
        perror("create new file");
        exit(1);
    }

    // Receive file contents from client
    // Loop continues till there is some bytes being sent (>0)
    while ((bytes_received = read(client_fd, &content, sizeof(content))) > 0) {
        
        // calculating total number of bytes that were received to print at end
        total_bytes_received += content.nbytes;


        // write the content into the new file
        if (write(fd, &content.payload, content.nbytes) == -1) {
            perror("write content into new file");
            exit(1);
        }

        // End of file
        if (content.nbytes < BUFSIZE) {    
            break;
        }
        
    }
    // close the file
    close(fd);

    // if there was error reading content from client
    if (bytes_received == -1) {
        perror("read content from client");
        exit(1);
    }

    // confirmation message that received everything
    printf("File transfer complete. %d bytes received.\n", total_bytes_received);
    
    // close fifo file descriptor
    close(client_fd);
    // unlink the fifo
    unlink(FIFO_NAME);
    return 0;
}
