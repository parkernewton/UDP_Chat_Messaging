// server_recv.c 
// Parker Newton 
// 4-16-15 
// COEN 146L - T 2:15 pm 

// This program implements socket programming to receive chat messages from a local client to a remote server. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define SERVER_PORT 10057
#define BUFFER_SIZE 1000

int main(int argc, char *argv[])
// Main driver function  
{
	int s; 
	char buffer[BUFFER_SIZE]; 
	struct sockaddr_in addr; 
	socklen_t addr_size = sizeof(addr); 

	// Establish the UDP socket 
	if((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		printf("\nError: cannot create a socket.\n"); 
		return -1; 
	} 

	// Write zeros to the members of the sockaddr_in struct 
	bzero((char *)&addr, sizeof(struct sockaddr_in));

	// AF_INET denotes address family for our socket as IP 
	addr.sin_family = AF_INET;

	// We accept any IP address to bind the socket to 
	addr.sin_addr.s_addr = INADDR_ANY; 

	// Specify the server port 
	addr.sin_port = htons(SERVER_PORT);

	// Bind socket to server IP addr and server port
	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		printf("\nError: server bind failed. %s\n", strerror(errno));   
		close(s); 
		return -1; 
	} 

	// We continuously wait until we receive a chat message
	while(1){
		while(!recvfrom(s, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addr_size)) 	continue; 
		if(buffer[0] == 'q' && buffer[1] == '\0'){
			// done 
			printf("\nClient 1: Bye!\n"); 
			break; 
		} 
		printf("\nClient 1: %s\n", buffer);
	} 
 
	close(s); 
	return 0; 
}