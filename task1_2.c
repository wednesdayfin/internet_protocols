// Simple daytime client -- modified from Stevens' example at
// intro/daytimetcpcli.c

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
    char student_id[7] = "219804\n";
    char task[11] = "1.2-binary\n";

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

   

    // Read data from socket, at most 80 (=MAXLINE) bytes
    // The result will appear in recvline
    // n contains number of bytes read, or 0 on endfile,
    // or < 0 on error
    int check1 = read(sockfd, recvline, MAXLINE);
    // If read return value was 0, loop terminates, without error

    //luodaan tarvittava structi

    struct numbers lol;

    //asetetaan lolin arvot, vai pitääkö typecastaa bufferi structiksi? 

    check2 = parse_str(recvline, &lol); //miksei käänny? struct lol on määritelty aiemmin

    lol.b = htonl(lol.b);
    lol.d = htons(lol.d);
    lol.e = htonl(lol.e);

    int check3 = write(sockfd, &lol.a, sizeof(lol.a));
    int check4 = write(sockfd, &lol.b, sizeof(lol.b));
    int check5 = write(sockfd, &lol.c, sizeof(lol.c));
    int check6 = write(sockfd, &lol.d, sizeof(lol.d));
    int check7 = write(sockfd, &lol.e, sizeof(lol.e));

    int rem_bytes = 12;
    char* ptr = recvline;

    while (rem_bytes > 0) {
        //recvline[n] = 0; // null terminate for printing purposes
        n = read(sockfd, ptr, rem_bytes);
        if (n == -1) {
            break;
        }
        rem_bytes -= n;
        ptr += n;
    }

    lol.a = recvline[0];
    lol.b = ntohl((uint32_t)recvline[1]);
    lol.c = recvline[5];
    lol.d = ntohs((uint16_t)recvline[6]);
    lol.e = ntohl((uint32_t)recvline[8]);

    output_str(recvline, MAXLINE, &lol);

    int check9 = read(sockfd, ptr, rem_bytes);

    return 0;
}
