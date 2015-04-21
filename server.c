// server.c 
// Parker Newton 
// 4-21-15 
// COEN 146L - T 2:15 pm 

// This program implements UDP socket programming to send chat messages from a remote server to a local client. 

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
// Expects the following program parameters: 
// 		1) name of remote server host. 

{
	// s stores the socket identifier 
	// buffer stores the C-string to send 
	// addr is a type-sockaddr struct that stores information about the address to which we send data 
	// srvr is type-hostent struct that stores information about the remote server endpoint 
	int s; 		
	char buffer[BUFFER_SIZE];  
	struct sockaddr_in addr; 		
	struct hostent *srvr; 				 		

	if(argc < 2){
		printf("\nError: not enough args to program.\n"); 
		return -1; 
	}

	// Establish a UDP socket 
	if((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		printf("\nError: cannot create a socket.\n"); 
		return -1; 
	}

	// Locate the server host 
	if((srvr = gethostbyname(argv[1])) == NULL){
		printf("\nError: could not verify host.\n");  
		close(s);
		return -1; 
	} 

	// Write 0's to the members of the sockaddr_in struct
	bzero((char *)&addr, sizeof(struct sockaddr_in)); 
 
	// AF_INET denotes address family for our socket as IP 
	addr.sin_family = AF_INET; 

	// Specify the IP address of the server 
	bcopy((char *)srvr->h_addr, (char *)&addr.sin_addr, srvr->h_length); 

	// Specify the server port 
	addr.sin_port = htons(SERVER_PORT); 

	// Read the data from the stdin and send via UDP function call
	printf("\nEnter the text to send to the remote host, or 'q' to quit. \n"); 
	while(scanf( " %[^\n][BUFFER_SIZE]", buffer)){
		printf("\nClient 2:\t%s \n", buffer); 
		if(sendto(s, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&addr, sizeof(addr)) != (strlen(buffer)+1)){
			printf("\nError sending packet to server.\n"); 
			close(s); 
			return -1; 
		} 
		if(buffer[0] == 'q' && buffer[1] == '\0') 	break; 
	}

	// Close the socket 
	close(s); 	
	return 0; 
}