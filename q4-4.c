#include<stdio.h>
#include<netdb.h>
#include<string.h>      //strlen
#include<sys/socket.h>
#include<arpa/inet.h>   //inet_addr
#include<unistd.h>      //write
#include<netinet/in.h>
#include<sys/types.h>
#include<stdlib.h>
#define MAX 80

// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, MAX); 
  
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
        bzero(buff, MAX); 
        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
  
        // and send that buffer to client 
        write(sockfd, buff, sizeof(buff)); 
  
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 

int main(int argc , char *argv[])
{
        int socket_desc , new_socket , c;
        struct sockaddr_in server , client;
        char *message;
        int optval;
        socklen_t optlen = sizeof(optval);

        //Create socket
        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (socket_desc == -1)
        {
                printf("Could not create socket");
        }


      /* Check the status for the keepalive option */
       if(getsockopt(socket_desc, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
        perror("getsockopt()");
        close(socket_desc);
        exit(EXIT_FAILURE);
        }
        printf("SO_KEEPALIVE is %s\n", (optval ? "ON" : "OFF"));

        /* Set the option active */
        optval = 1;
        optlen = sizeof(optval);
        if(setsockopt(socket_desc, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
          perror("setsockopt()");
          close(socket_desc);
          exit(EXIT_FAILURE);
         }
         printf("SO_KEEPALIVE set on socket\n");
         
       /* Check the status again */
       if(getsockopt(socket_desc, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
       perror("getsockopt()");
       close(socket_desc);
       exit(EXIT_FAILURE);
       }
       printf("SO_KEEPALIVE is %s\n", (optval ? "ON" : "OFF"));

        //Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( 8888 );
        //Bind
        if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        {
                puts("bind failed");
                return 1;
        }
        puts("bind done");

        //Listen
        listen(socket_desc , 3);

        //Accept and incoming connection
        puts("Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);
        new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

        if (new_socket<0)
        {
                perror("accept failed");
                return 1;
        }
        printf("Server accept the client...\n");
        // Function for chatting between client and server 
        func(new_socket); 
        close(socket_desc);
        return 0;
}
