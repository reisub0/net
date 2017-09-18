/*
 * tcpclient.c - A simple TCP client
 * usage: tcpclient <host> <port>
 */
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 1024

void except(char *msg) {
	perror(msg);
	exit(0);
}

int main(int argc, char **argv) {
	int sockfd, portno, n;
	struct sockaddr_in serveraddr;
	struct hostent *server;
	char *hostname;
	char buf[BUFSIZE];

	/* check command line arguments */
	if (argc < 3) {
		fprintf(stderr, "usage: %s <hostname> [port=4567]\n", argv[0]);
		exit(0);
	}
	hostname = argv[1];
	portno = 4567;
	if (argc == 3) {
		portno = atoi(argv[2]);
	}
	/* socket: create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		except("ERROR opening socket");

	/* gethostbyname: get the server's DNS entry */
	server = gethostbyname(hostname);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host as %s\n", hostname);
		exit(0);
	}

	/* build the server's Internet address */
	bzero((char *)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr,
				server->h_length);
	serveraddr.sin_port = htons(portno);

	/* connect: create a connection with the server */
	if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0)
		except("ERROR connecting");

	/* get message line from the user */
	printf("Please enter msg: ");
	bzero(buf, BUFSIZE);
	fgets(buf, BUFSIZE, stdin);

	/* send the message line to the server */
	n = write(sockfd, buf, strlen(buf));
	if (n < 0)
		except("ERROR writing to socket");

	/* print the server's reply */
	bzero(buf, BUFSIZE);
	n = read(sockfd, buf, BUFSIZE);
	if (n < 0)
		except("ERROR reading from socket");
	printf("Echo from server: %s", buf);
	close(sockfd);
	return 0;
}
