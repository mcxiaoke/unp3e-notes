#include <stdio.h>
#include <unp.h>
#include "sum.h"

void str_cli2(FILE *fp, int sockfd){
	char sendline[MAXLINE];
	struct args args;
	struct result result;
	while (fgets(sendline, MAXLINE, fp)!=NULL) {
		if (sscanf(sendline, "%ld%ld", &args.arg1, &args.arg2) != 2) {
			printf("invalid input: %s", sendline);
			continue;
		}
		write(sockfd, &args, sizeof(args));
		if (read(sockfd, &result, sizeof(result)) == 0) {
			err_quit("str cli bin, server terminated.");
		}
		printf("%ld\n", result.sum);
	}
}

void str_cli(FILE *fp, int sockfd){
	char sendline[MAXLINE], recvline[MAXLINE];
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		write(sockfd, sendline, strlen(sendline));
//		write(sockfd, sendline, 1);
//		sleep(1);
//		write(sockfd, sendline+1, strlen(sendline)-1);
		if (readline(sockfd, recvline, MAXLINE) == 0) {
			err_quit("str cli server terminated.");
		}
		fputs(recvline, stdout);
	}
}

int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in server;
	if (argc != 2) {
		err_quit("usage: client server_ip");
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		err_quit("socket error");
	}
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &server.sin_addr);
	if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		err_sys("connect error");
	}
	str_cli2(stdin, sockfd);
	printf("connection closed.\n");
	return 0;
}