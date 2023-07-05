/*
# --------------------------------------------------------------------------------------------------------------------
#   Client Server Processes
#
#   Name: server.c 
#
#   Written By: Punyaja Mishra - March 2023
#
#   Purpose: This program acts as the server that will communicate with the client.c
#   program. The programs use Sockets to communicate with each other. The program will
#   run in the background first, folllowed by execution of the client code, by sending 
#   a filepath as the argument. The server reads the filename and creates a new file
#   with "_server" at the end. The it keeps reading the file in blocks of 1024 and stops
#   when its less than 1024 because it means we are at the end of the file
#   
#
#   usage: gcc server.c -o server_o
#          eg: ./server_o &
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
#include <stdlib.h>
#include <signal.h> 
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// socket > bind > listen > accept > read > write > close

// size of the string being added to the end of the filename
#define STR_SIZE 7
// max block size to be written in file
#define MAX_R 1024
// max size for filename
#define MAX_FILESIZE 512
// max char array length for disconnect msg
#define MAX_MSG_LEN 10

// struct to store file contents being read from client and written in the new file
typedef struct student_record 
{ 
    int nbytes; 
    char payload[MAX_R]; 
}st_rec; 

int newfd;                  // returned by accept() 

// main program
int main(int args, char* argv[]){
    int socketd;                //socket descriptor
    struct sockaddr_in srv;     // used by bind() to get socket address, internet style
    struct sockaddr_in cli;     // used by accept()
    int cli_len = sizeof(cli);  // size of the structure of client
    
    // disconnect message received from client
    char disconnect_msg[MAX_MSG_LEN];
    // string to append at the end of the file name
    char _server[STR_SIZE] = "_server";

    // character array to store filename
    char buf[MAX_FILESIZE];  

    // the file we create
    int outfile; 
    // struct to store file content
    st_rec r_content;

    // creating socket 
    if((socketd=socket(AF_INET, SOCK_STREAM,0))<0){
        perror("socket create failed\n");
        exit(1);
    }

    // use the internet addr family
    srv.sin_family = AF_INET;
    //bind socket to port 50,000 + 6001 = 56001
    srv.sin_port = htons(56001);
    // bind: a client may connect to any of my addresses  on local machine
    srv.sin_addr.s_addr = htonl(INADDR_ANY);  
    
    
    // binding socket to port
    if(bind(socketd,(struct sockaddr*) &srv, sizeof(srv))<0){
        perror("bind failed\n");
        exit(1);
    }

    // listening - server accepting a connection
    if(listen(socketd,5)<0){
        perror("listen failed\n");
        exit(1);
    }

    // accepting from client
    newfd = accept(socketd, (struct sockaddr*) &cli, &cli_len);
    if(newfd < 0){
        perror("accept failed\n");
        exit(1);
    }

    // read the file name 
    if(read(newfd, buf, sizeof(buf))<0){
        perror("read filename failed\n");
        exit(1);
    }
    // add a "_server" to the end of the filename string
    strncat(buf,_server,STR_SIZE);
    
    //create the file with the filename we made above
    if((outfile = open(buf, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        perror("create and open new file failed\n");
        exit(1);
    }

    //read the struct being sent by client till the block size is less than MAX_R which means we are at end of file
    do{
        //read data
        if(read(newfd,&r_content,sizeof(r_content))<0){
            perror("read file content failed\n");
            exit(1);
        }
        printf("reading struct: %ld \n", sizeof(r_content));
        printf("reading nbytes: %d \n", r_content.nbytes);
        //write data into new file
        if(write(outfile,r_content.payload,r_content.nbytes)<MAX_R){
            close(outfile);     
            exit(0);
        }
    }while(r_content.nbytes >= MAX_R);

    
    // receive disconnect message
    recv(newfd,&disconnect_msg,MAX_MSG_LEN,0);

    // if received message is to disconnect then print disconnecting 
    //and send client message that hey got it
    if(strncmp(disconnect_msg,"disconnect",MAX_MSG_LEN)){
        printf("Received msg from client - Disconnecting\n");
        send(newfd,&disconnect_msg,MAX_MSG_LEN,0);
       
    }
     
    //close
    close(newfd);
    close(socketd);

return 0;
}