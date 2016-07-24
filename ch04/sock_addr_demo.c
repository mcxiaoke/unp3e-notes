#include <stdio.h>
#include <unp.h>
#include <time.h>

void print_addr(struct sockaddr_in *addr){
	char buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(addr->sin_addr), buf, sizeof(buf));
	printf("%s:%d\n", buf, ntohs(addr->sin_port));
}

// need run using sudo, as port = 13
int main(int argc, char *argv[]) {
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	socklen_t len;
	char buf[MAXLINE];
	time_t ticks;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		err_sys("create socket error");
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9999);
	if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		err_sys("bind socket error");
	}
	struct sockaddr_in local;
	len = sizeof(local);
	getsockname(listenfd, (struct sockaddr *)&local, &len);
	printf("bind and listen on ");
	print_addr(&local);
	if (listen(listenfd, LISTENQ) < 0){
		err_sys("listen socket error");
	}
	struct sockaddr_in client;
	while (1) {
		len = sizeof(client);
		printf("waiting next connection...\n");
		connfd = accept(listenfd, (struct sockaddr *)&client, &len);
		printf("connection from ");
		print_addr(&client);
		ticks = time(NULL);
		snprintf(buf, sizeof(buf),"Time: %.24s\r\n", ctime(&ticks));
		write(connfd, buf, strlen(buf));
		close(connfd);
	}
	printf("server closed.\n");
}