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




void handleRequest(int fdClient, char buf[])
{

    char *token = strtok(buf, " ");
 
    while (token != NULL) 
    {
        //check request type
        if(strcmp(token,"GET")==0)
        {
            token = strtok(NULL, " ");

            if(*token=='/')
                token++;
            
            int fdfile =  open(token, O_RDONLY);

            if(fdfile == -1)
            {
                
                
                char responseHeader[] =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html; charset = UTF-8\r\n\r\n";

                write(fdClient,responseHeader, sizeof(responseHeader) - 1);
                fdfile = open("error.html",O_RDONLY);

            }
            else
            {
                char responseHeader[] =
                "HTTP/1.1 200 OKr\n"
                "Content-Type: text/html; charset = UTF-8\r\n\r\n";

                write(fdClient,responseHeader, sizeof(responseHeader) - 1);
                fdfile = open(token, O_RDONLY);
               
                
            }
             //TODO:filesize
                
			sendfile(fdClient, fdfile, NULL, 318);
            close(fdfile);
            break;

        }
        
    }

}
int main(int argc, char *argv[])
{
	struct  sockaddr_in servAddr,cliAddr; //structure for handling internet addresses
	socklen_t sin_len = sizeof(cliAddr); //socklen_t is a typedef-an int for containing atleast 32 bit length addresses
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
			
            handleRequest(fdClient,buf);

			
			close(fdClient);
			printf("closing..\n");
			exit(0);
		}
		close(fdClient);
	}
	return 0;
}
