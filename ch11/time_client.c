#include <stdio.h>
#include <unp.h>
#include "sockaddr.h"
#include "tcp.h"

int main(int argc, char *argv[]) {
	int sockfd, n;
	char recvline[MAXLINE+1];
	socklen_t len;
	struct sockaddr_storage ss;
	if (argc != 3) {
		err_quit("usage: cmd hostname/ip service/port");
	}
	sockfd = tcp_connect(argv[1], argv[2]);
	len = sizeof(ss);
	getpeername(sockfd, (struct sockaddr *)&ss, &len);
	printf("connected to %s\n", sockaddr_ntop((struct sockaddr *)&ss));
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;
		fputs(recvline, stdout);
	}
	return 0;
}