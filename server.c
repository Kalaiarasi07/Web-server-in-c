#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define PORT 631
#define BUF_SIZE 100
#define SOCADDR struct sockaddr
void serverChat(int fd)
{
	char buf[BUF_SIZE];
	while(1)
	{
		bzero(buf,BUF_SIZE);

		//read the message from client into buf
		read(fd,buf,sizeof(buf));
		
		//print buffer
		printf("Client says: %s\nServer says:",buf);
		bzero(buf,BUF_SIZE);

		//get server message in buffer
		int n=0;
		while ((buf[n++] = getchar()) != '\n')
            		;
		//send buffer to client
		write(fd, buf, sizeof(buf));

		//handle exit case
		if(strncmp("exit",buf,4) == 0){
			printf("Server exits\n");
			break;
		}
	}
}
int main()
{
	//create socket
	
	int soc;
	soc = socket(AF_INET,SOCK_STREAM,0);
	/* AF_INET = address format
	 * SOC_STREAM = bi-directional connection mode byte streams
	 * 0 - default apt protocol for the given type will be used
	 */
	if(soc<0){
		printf("Socket creation failed!\n");
		exit(0);
	}
	else{

		printf("Socket creation successful!\n");
	}
	
	
	//sockaddr_in is a structure that handles internet addresses
	struct sockaddr_in servAddress;
	bzero(&servAddress,sizeof(servAddress));

	//assign ip,port

	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	/*htonl() & htons converts from host to network byte order
	 * INADDR_ANY - ipv4 of local host
	 */
	servAddress.sin_port = htons(PORT);
	
	//bind socket with ip,port
	
	int b = bind(soc, (SOCADDR*)&servAddress, sizeof(servAddress));
	if(b<0){
		printf("Socket binding failed!\n");
		exit(0);
	}
	else{
		printf("Socket binding successful\n");
	}

	//check if server is listening
	int l = listen(soc,3);//3-backlog used to limit no. of standing connections
	if(l<0){
		printf("Server is not listening!\n");
		exit(0);
	}
	else{
		printf("SErver is listening!\n");
	}

	//accept data packet from client
	struct sockaddr_in cliAddress;
	int len = sizeof(cliAddress);
	int fd = accept(soc, (SOCADDR*)&cliAddress , &len);//returns a file descriptor of accepted socket

	if(fd<0){
		printf("Data packet from client not accepted!\n");
	}
	else{
		printf("Data packet from client not accepted!\n");
	}

	//function for chat
	serverChat(fd);

	//close socket
	close(soc);

	return 0;
}
