/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void serveFile(int sock)
{
	char buffer[256];
	//TODO : change the block size of reading
	bzero(buffer, 256); 
    
	/* Read file requesst from client */
    int bytes_read = read(sock, buffer, 255);
    if (bytes_read < 0)
    	error("ERROR reading from socket\n");
    
    /* extract filename */
    char* filename = (char*)malloc(strlen(buffer) - 3);
    strncpy(filename, buffer + 4, strlen(buffer) - 3);

    /* Open the requested file */
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)	// handle this in client
    	error("ERROR file not found\n");

    
    /* Send requested file */
    printf("Sending file %s to client %d\n", filename, sock);

    while(1)
    {
    	int bytes_read = fread(buffer, sizeof(char), sizeof(buffer), fp);
    	if(bytes_read > 0)
    	{
    		int bytes_sent = send(sock, buffer, bytes_read, 0);
    		if (bytes_sent < bytes_read) 
    			error("ERROR writing to socket\n");
    	}
    	if(bytes_read == 0)
    	{
    		printf("File %s successfully sent to client %d\n",filename,sock);
    		fclose(fp);
    		break;
    	}
    	if(bytes_read < 0)
    	{
    		error("ERROR reading from file\n");
    	}
    }
    close(sock);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen, yes = 1;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2 || argc > 2) {
        fprintf(stderr,"usage :  %s [port]\n", argv[0]);
        exit(1);
    }

    /* create socket */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (sockfd <= 0) 
        error("ERROR opening socket\n");

    /* fill in port number to listen on. IP address can be anything (INADDR_ANY) */

    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* bind socket to this port number on this machine */

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
    error("ERROR on binding\n");
    
    /* listen for incoming connection requests */


    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    /* accept a new request, create a newsockfd */

	while (1)
	{
	    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
	    if (newsockfd < 0) 
	        error("ERROR on accept\n");
	    printf("Client %d connected\n", newsockfd);

	    pid_t pid = fork();

	    if (pid < 0)
	    {
	    	error("ERROR could not fork new process\n");
	    }
	    if(pid == 0)
	    {
	    	close(sockfd);
	    	serveFile(newsockfd);
	    }
	    else
	    {
            // Reap processes periodically
            // NULL is for getting argument for status
            while (waitpid(-1, NULL , WNOHANG)>0)
        	{
        		printf("Child process terminated\n");
        	}
	    	
	    	close(newsockfd);
	    }
	}
	//wait();
    return 0; 
}
