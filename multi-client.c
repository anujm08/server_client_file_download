#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3 || argc > 3) {
       fprintf(stderr, "usage :  %s [host] [port]\n", argv[0]);
       exit(0);
    }

    /* create socket, get sockfd handle */

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket\n");

    /* fill in server address in sockaddr_in datastructure */

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* connect to server */

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting\n");
    printf("A client connected to the server\n", sockfd);

    /* temporary get user input */
    printf("Please enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    /* send user message to server */

    int bytes_sent = write(sockfd, buffer, strlen(buffer));
    if (bytes_sent < 0) 
         error("ERROR writing to socket\n");
    bzero(buffer, 256);

    while(1)
    {
        int bytes_recv = recv(sockfd, buffer, sizeof(buffer), 0);
        
        if (bytes_recv < 0) 
            error("ERROR reading from socket\n");
        if (bytes_recv == 0)
        {
            printf("File received\n");
            break;
        }
    }
    return 0;
}
