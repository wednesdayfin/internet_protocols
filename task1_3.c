// Simple daytime client -- modified from Stevens' example at
// intro/daytimetcpcli.c

#include <sys/socket.h>  // defines socket, connect, ...
#include <netinet/in.h>  // defines sockaddr_in
#include <string.h>      // defines memset
#include <stdio.h>       // defines printf, perror, ...
#include <arpa/inet.h>   // inet_pton, ...
#include <unistd.h>      // read, ...
#include <errno.h>       // ernot
#define MAXLINE 100

int main(int argc, char **argv)
{
    int sockfd, n, check2; //täällä määritellään
    n = 1;
    char recvline[MAXLINE + 1];
    char sendline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    char student_id[] = "219804\n";
    char task[] = "1.3-large\n";

    if (argc != 2) {
        fprintf(stderr, "usage: a.out <IPaddress>\n");
        return 1;
    }

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

    if (connect(sockfd,
                (struct sockaddr *) &servaddr,
                sizeof(servaddr)) < 0) {
        perror("connect error");
        return 1;
    }

    write(sockfd, student_id, 7);

    write(sockfd, task, 10);

    int obj_size = 1;

    while (obj_size > 0) {
        n = read(sockfd, &obj_size, 4);
        obj_size = ntohl(obj_size);
        int total = obj_size;
        if (obj_size == 0) {
            int i = 0;
            write(sockfd, &i, sizeof(i));
            read(sockfd, &i, sizeof(i));
            return 0;
        }
        while (obj_size > 0) {
            
            obj_size -= read(sockfd, recvline, MAXLINE);
            
        }

        total = htonl(total);
        write(sockfd, &total, sizeof(obj_size));
        obj_size = 1;
    }

    return 0;
}
