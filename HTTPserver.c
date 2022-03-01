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

char response[] =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset = UTF-8\r\n\r\n"
"<!DOCTYPE Html>\r\n"
"<html><head><title>Requested page</title></head>\r\n"
"<body><center><h1>It works! </h1><br>\r\n"
"<h3>not really</h3><br>\r\n"
"<img src=\"img.png\"></center></body></html>\r\n";

int main(int argc, char *argv[])
{
	struct  sockaddr_in servAddr,cliAddr; //structure for handling internet addresses
	socklen_t sin_len = sizeof(cliAddr);
	int fdServer,fdClient; //socket file descriptors

	char buf[2048];
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
		       	memset (buf, 0, 2048);// fills 0 to 2048 bytes of buf
		       	read(fdClient, buf, 2047); //reads 2047 bytes of data from fdclient into buf
			
			printf("---------HTTP Request-------\n%s\n", buf);
			//TODO: if img file exists
			char *file = "/home/user/img.png";
			if( access( file, F_OK ) == 0 ) {
			    printf("exists\n");
			} else {
    				printf("doesnt exist\n");
			}
			if ((strncmp (buf, "GET /img.png", 12))==0) //compares first 12 characters of http request with string
			{
				fdimg =  open("img.png", O_RDONLY);
			       	sendfile(fdClient, fdimg, NULL, 4000);// copies 4000 bytes from fdimg to fdclient
			       	close(fdimg);
			}
			else
				write(fdClient,response, sizeof(response) - 1);
			close(fdClient);
			printf("closing..\n");
			exit(0);
		}
		close(fdClient);
	}
	return 0;
}
