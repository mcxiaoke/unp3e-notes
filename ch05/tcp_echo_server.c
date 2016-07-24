#include <stdio.h>
#include <unp.h>
#include "sum.h"

void sig_child(int signo){
	pid_t pid;
	int stat;
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		printf("child %d terminated\n", pid);
	}
	return;
}

void str_plus(int sockfd){
	long arg1, arg2;
	ssize_t n;
	char line[MAXLINE];
	for(;;){
		if ((n = readline(sockfd, line, MAXLINE)) == 0) {
			return;
		}
		if (sscanf(line, "%ld%ld", &arg1, &arg2) == 2) {
			snprintf(line, sizeof(line), "%ld\n", arg1 + arg2);
		}else {
			snprintf(line, sizeof(line), "input error\n");
		}
		n = strlen(line);
		write(sockfd, line, n);
	}
}

void str_echo2(int sockfd){
	ssize_t n;
	struct args args;
	struct result result;
	for(;;){
		if ((n = read(sockfd, &args, sizeof(args))) == 0) {
			return;
		}
		result.sum = args.arg1 + args.arg2;
		write(sockfd, &result, sizeof(result));
	}
}

void str_echo(int sockfd){
	ssize_t n;
	char buf[MAXLINE];
	again:
		while ((n = read(sockfd, buf, MAXLINE)) > 0) {
			write(sockfd, buf, n);
			if (n < 0 || errno == EINTR) {
				goto again;
			}else if (n < 0) {
				err_sys("str echo read error");
			}
		}
}

int main(int argc, char *argv[]) {
	int listenfd, connfd;
	pid_t child;
	socklen_t clen;
	struct sockaddr_in client, server;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		err_sys("socket error");
	}
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(SERV_PORT); // 9877
	if (bind(listenfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		err_sys("bind error");
	}
	listen(listenfd, LISTENQ);
	Signal(SIGCHLD, sig_child);
	for (;;) {
		clen = sizeof(client);
		connfd = accept(listenfd, (struct sockaddr *)&client, &clen);
		if (connfd < 0) {
			// restart accpet.
			if (errno == EINTR) {
				continue;
			}else{
				err_sys("accept error");
			}
		}else{
			if ((child = fork()) == 0) {
				// child process
				close(listenfd);
				str_echo2(connfd);
				exit(0);
			}else{
				// parent process
				close(connfd);
			}
		}
	}
	printf("server closed.\n");
}