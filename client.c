#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100
#define PORT 631
#define SA struct sockaddr
void clientChat(int fd)
{
	char buf[BUF_SIZE];
	int n;
	while(1) {
		bzero(buf, sizeof(buf));
		printf("Client says : ");
		n = 0;
		while ((buf[n++] = getchar()) != '\n')
			;
		write(fd, buf, sizeof(buf));
		bzero(buf, sizeof(buf));
		read(fd, buf, sizeof(buf));
		printf("Server says : %s", buf);
		if ((strncmp(buf, "exit", 4)) == 0) {
			printf("Client Exits...\n");
			break;
		}
	}
}

int main()
{
	int soc;

	// socket creation
	soc = socket(AF_INET, SOCK_STREAM, 0);
	if (soc==-1) {
		printf("socket creation failed!\n");
		exit(0);
	}
	else
		printf("Socket creation successful!\n");
	struct sockaddr_in servAddress;
	bzero(&servAddress, sizeof(servAddress));

	// assign IP, PORT
	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = inet_addr("127.0.0.1");//Converts string to inet address
	servAddress.sin_port = htons(PORT);

	// connecting client socket to server socket
	int con = connect(soc, (SA*)&servAddress, sizeof(servAddress));
	if(con!=0)	{
		printf("connection with server failed\n");
		exit(0);
	}
	else
		printf("connection with server successful\n");

	// function for chat
	clientChat(soc);

	// close the socket
	close(soc);
}

