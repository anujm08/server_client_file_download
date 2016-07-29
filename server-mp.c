/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2 || argc > 2) {
        fprintf(stderr,"usage :  %s [port]\n", argv[0]);
        exit(1);
    }

    /* create socket */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* fill in port number to listen on. IP address can be anything (INADDR_ANY) */

    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* bind socket to this port number on this machine */

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
    
    /* listen for incoming connection requests */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* accept a new request, create a newsockfd */

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");

    /* read message from client */

    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0) error("ERROR reading from socket");
    
    char* filename = calloc(1, strlen(buffer) - 4);
    memcpy(filename, buffer + 4, strlen(buffer) - 5);

    FILE *fp = fopen(filename,"r");
    //printf("%s",filename);

    if(fp == NULL)
    	error("ERROR file not found");

    printf("%s", filename);

    while(1)
    {
    	int bytes_read = fread(buffer, sizeof(char), sizeof(buffer), fp);
    	if(bytes_read > 0)
    	{
    		n = send(newsockfd, buffer, bytes_read, 0);
    		if (n < bytes_read) 
    			error("ERROR writing to socket");
    	}
    	if(bytes_read == 0)
    	{
    		fclose(fp);
    		break;
    	}
    	if(bytes_read < 0)
    	{
    		error("ERROR reading from file");
    	}
    }

    return 0; 
}
