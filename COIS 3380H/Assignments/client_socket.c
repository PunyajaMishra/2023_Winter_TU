/*
# --------------------------------------------------------------------------------------------------------------------
#   Client Server Processes
#
#   Name: client.c 
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: This program acts as the client that will communicate with the server.c
#   program. The programs use Sockets to communicate with each other. The server.c code
#   is run in the background first, folllowed by running the client code, by sending 
#   a filepath as the argument. The client when connects to server sends a message that is 
#   filename. Then it proceeds to open the file and copy it through the channel to the server 
#   using read() and write()
#   
#
#   usage: gcc client.c -o client_o
#          eg: ./client_o diplodocus.pdf
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
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h> 
#include <unistd.h>
#include <fcntl.h>

// socket > connect > write > read > close

// max block size to be read from file
#define MAX_R 1024
// max size for filename
#define MAX_FILESIZE 512
// max char array length for disconnect msg
#define MAX_MSG_LEN 10

// struct to store file information
typedef struct student_record 
{ 
    int nbytes;             //number of bytes/size
    char payload[MAX_R];     //content/characters of the file
} st_rec;

// main function
int main(int args, char* argv[]){
    int socketd;                    //socket descriptor
    struct sockaddr_in srv;         // used by connect

    //disconnect messagee
    char disconnect_msg[MAX_MSG_LEN] = "disconnect";

    // If a filename is not provided then print error and exit program
    if(args < 2){
        printf("Missing argument - filename\n");
        exit(1);
    }

    // character array to store the name of the file 
    char filename[MAX_FILESIZE];
    //get the filename
    strncpy(filename,argv[1],strlen(argv[1]) + 1); 
    // Pointer to the file
    char *inputFile = argv[1];  
    //struct to store the file information 
    st_rec content; 
    // read file name and send file - number of characters
    int infile;

    // creating socket to connect
    if((socketd=socket(AF_INET, SOCK_STREAM,0))<0){
        perror("socket create failed\n");
        exit(1);
    }

    // use the internet addr family
    srv.sin_family = AF_INET;
    //bind socket to port 50,000 + 6001 = 56001 
    srv.sin_port = htons(56001);
    // connect to an address
    srv.sin_addr.s_addr = inet_addr("192.197.151.116");  
    
    //connecting to server, print error if failed
    if(connect(socketd, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("connect call failed\n"); 
        exit(1);
    }
    
    // send the file name to the server, print error if failed
    if(write(socketd, filename, strlen(filename)) < 0)
    {
        perror("write filename failed\n");
        exit(1);
    }

    //open file and store in infile
    if((infile = open(inputFile, O_RDONLY)) == -1){
        perror("open file failed\n");
        exit(1);
    }
    
    // read all contents of file and write it in the struct variable - content
    // write this struct to the server
    // content.nbytes = amount of data read
    // content.payload = content read/my buffer
    do{
        if((content.nbytes = read(infile,content.payload,MAX_R))<0){
            perror("read file client failed\n");
            exit(1);
        }
        printf("writing nbytes: %d \n", content.nbytes);
        printf("writing struct: %ld \n", sizeof(content));
        write(socketd,&content,sizeof(content));
        // write the struct to the server, if less than MAX SIZE then close the file 
        if(content.nbytes<MAX_R){
            close(infile);
            exit(0);
        }
    }while(content.nbytes >= MAX_R);

    
    // all contents of file has been read so send disconnect message
    send(socketd,&disconnect_msg,MAX_MSG_LEN,0);

    // receive message that server got disconnect
    recv(socketd,&disconnect_msg,MAX_MSG_LEN,0);
    
    // if received message is to disconnect then print disconnecting 
    if(strncmp(disconnect_msg,"disconnect",MAX_MSG_LEN)){
        printf("Received msg from server - Disconnecting\n");
    }

    //close
    close(socketd);

return 0;
}