// Simple daytime client -- modified from Stevens' example at
// intro/daytimetcpcli.c

#include <sys/socket.h>  // defines socket, connect, ...
#include <netinet/in.h>  // defines sockaddr_in
#include <string.h>      // defines memset
#include <stdio.h>       // defines printf, perror, ...
#include <arpa/inet.h>   // inet_pton, ...
#include <unistd.h>      // read, ...

#define MAXLINE 80

int main(int argc, char **argv)
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    // Requires IPv4 address as a command line argument
    if (argc != 2) {
        fprintf(stderr, "usage: a.out <IPaddress>\n");
        return 1;
    }

    // Open a stream (TCP) IPv4 socket, and check if succesful
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(5000); /* daytime server = 13 */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        return 1;
    }

    // Connect to IP address and port indicated by servaddr
    // Check if it was succesful
    if (connect(sockfd,
                (struct sockaddr *) &servaddr,
                sizeof(servaddr)) < 0) {
        perror("connect error");
        return 1;
    }

    // Read data from socket, at most 80 (=MAXLINE) bytes
    // The result will appear in recvline
    // n contains number of bytes read, or 0 on endfile,
    // or < 0 on error
    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0; // null terminate for printing purposes

        //  output to stdout (e.g., terminal display)
        if (fputs(recvline, stdout) == EOF) {
            fprintf(stderr, "fputs error\n");
            return 1;
        }
    }

    // If read return value was 0, loop terminates, without error
    if (n < 0) {
        perror("read error");
        return 1;
    }
    return 0;
}
