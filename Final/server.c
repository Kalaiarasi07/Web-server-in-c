#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include "postparser1.h"
#define FAILURE "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset = UTF-8\r\n\r\n"
#define SUCCESS "HTTP/1.1 200 OKr\nContent-Type: text/html; charset = UTF-8\r\n\r\n"

//function to return filesize
off_t filesize(int fdfile)
{
	struct stat buf;
	fstat(fdfile, &buf);
	off_t size = buf.st_size;
	return size;
}

//parses http request and sends response
void handleRequest(int fdClient, char buf[])
{
	// printf("buffer:%s\n",buf);
	char buf1[10000];
	strcpy(buf1,buf);

    char *token = strtok(buf, " "); //gets first token i.e., request type

    while (token != NULL) 
    {
        //GET request handling
        if(strcmp(token,"GET")==0)
        {
            token = strtok(NULL, " "); // gets file path

            if(strcmp(token,"/")==0 ) //if root, index.html must be returned
                token = "index.html";
			else 
				token++;
            
            int fdfile =  open(token, O_RDONLY); //open file

			printf("\n\n\nFndFILE: %d\n\n\n",fdfile);
            if(fdfile == -1) //if file not found return 404 error
            {
                write(fdClient,FAILURE, sizeof(FAILURE)-1); 
            }
            else //return 200 status and file
            {
                write(fdClient,SUCCESS , sizeof(SUCCESS)-1); 
                fdfile = open(token, O_RDONLY);
                sendfile(fdClient, fdfile, NULL, filesize(fdfile));           
            }
			close(fdfile); 
            break;
        }

		else if(strcmp(token,"POST")==0)
		{
			const char *delim = "\r\n\r\n";
			
			

			//get content type
			char contentType[100];
    
			char *conType = strstr(buf1,"Content-Type: ");
			conType += strlen("Content-Type: ");
			int i = 0;
			while ( *conType != '\r' && *conType != ';')
			{
				contentType[i++] = *conType;
				conType++;
			}
			contentType[i] = '\0';
			// printf("contenttype:\n%s\n",contentType);

						
			//get body of request
			char *body;
			body = strstr(buf1, delim);
			body = body+strlen(delim);
			// printf("BODY:\n\n%s\n\n", body);
			
			

			//based on content type and call apt functions
			int parse_status;
			if( strcmp(contentType,"application/x-www-form-urlencoded") == 0 )
			{
				parse_status = applicationtype_parser(body);
			}
			else if(strcmp(contentType,"multipart/form-data") == 0)
			{

				parse_status = multiparttype_parser(body);
			}
			if(parse_status) //parsing successful
			{
				write(fdClient,SUCCESS, sizeof(SUCCESS)-1);
			}
			else
			{
				write(fdClient,FAILURE, sizeof(FAILURE)-1);
			}	
			break;
			
		}        
    }
	

}
int main(int argc, char *argv[])
{
	struct  sockaddr_in servAddr,cliAddr; //structure for handling internet addresses
	socklen_t sin_len = sizeof(cliAddr); //socklen_t is a typedef-an int for containing atleast 32 bit length addresses
	int fdServer,fdClient; //socket file descriptors

	char buf[10000];
	int fdimg;
	int on =1;
	
	//socket creation
	fdServer = socket (AF_INET, SOCK_STREAM, 0); 
	if(fdServer < 0)
	{
		perror ("socket");
	       	exit(1);
	}

	//configuring socket properties
	/*SOL_SOCKET - sets the option(SOL_REUSEADDR) at socket level;
	 * SO_REUSEADDR - Specifies that the rules used in validating addresses supplied to bind() should allow reuse of local addresses, if this is supported by the protocol. This option takes an int value. This is a Boolean option.
	 * on - sets the option to the value pointed by on
	 */
	setsockopt (fdServer, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (int));

	//set IP and PORT
		servAddr.sin_family= AF_INET;
       	servAddr.sin_addr.s_addr = INADDR_ANY;
       	servAddr.sin_port= htons (8080);

	// binds server to socket
	if (bind (fdServer, (struct sockaddr *) &servAddr, sizeof (servAddr)) == -1)
	{
		perror ("bind");
	       	close(fdServer);
	       	exit(1);
	}

	//make server listen
	if(listen (fdServer, 10) == -1)
	{
		perror ("listen");
	       	close(fdServer);
		exit(1);
	}

	while(1)
	{
		//accept data packet from client
		fdClient = accept (fdServer, (struct sockaddr *) &cliAddr, &sin_len);
		if(fdClient == -1)
		{
			perror ("Connection failed....\n"); 
			continue;
		}
		printf("Got client connection...... \n");


		if (!fork())
		{
			//child process
			close(fdServer);
		       	memset (buf, 0, 2048);
		       	read(fdClient, buf, 2047); 
			
			printf("---------HTTP Request-------\n%s\n", buf);
            handleRequest(fdClient,buf);

			
			close(fdClient);
			printf("closing..\n");
			exit(0);
		}
		close(fdClient);
	}
	return 0;
}
