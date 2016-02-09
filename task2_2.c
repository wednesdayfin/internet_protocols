#include <sys/socket.h>  // defines socket, connect, ...
#include <netinet/in.h>  // defines sockaddr_in
#include <string.h>      // defines memset
#include <stdio.h>       // defines printf, perror, ...
#include <arpa/inet.h>   // inet_pton, ...
#include <unistd.h>      // read, ...
#include <errno.h>       // ernot
#include "template.h"

#define MAXLINE 100

int main(int argc, char **argv)
{
    int sockfd, n, check2; //täällä määritellään
    char recvline[MAXLINE + 1];
    char sendline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    char student_id[] = "219804\n";
    char task[] = "1.2-binary\n";

    printf("hello");
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

    printf("socket created");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(5000); /* serverin numero */
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

    printf("connected");

    write(sockfd, student_id, 7);

    write(sockfd, task, 11);
    
    printf("Moro\n");

    // luetaan ip-osoite ja portti, joko ipv4 tai ipv6
    int check1 = read(sockfd, recvline, MAXLINE);
    
    //avataan secondary socket
    
    //lahetetaan local address ja portti secondaryyn serverille ADDR <ip address> <port> <student ID> <newline>
    
    //luetaan seuraava line main socketista, ehka monta CONN-messagea
    //jokaiseen otetaan yhteys
    
    //OK
    
    //printf(recvline);

    return 0;
}
