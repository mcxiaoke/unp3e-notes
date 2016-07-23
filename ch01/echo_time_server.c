#include <stdio.h>
#include <unp.h>
#include <time.h>

// need run using sudo, as port = 13
int main(int argc, char *argv[]) {
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	time_t ticks;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		err_sys("create socket error");
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13); // ntp port
	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	listen(listenfd, LISTENQ);
	struct sockaddr_in client;
	char client_addr[INET_ADDRSTRLEN];
	while (1) {
		socklen_t client_size = sizeof(client);
		connfd = accept(listenfd, (struct sockaddr *)&client, &client_size);
		inet_ntop(AF_INET, &client.sin_addr, client_addr, INET_ADDRSTRLEN);
		printf("connection from %s:%d\n", client_addr, ntohs(client.sin_port));
		ticks = time(NULL);
		snprintf(buf, sizeof(buf),"Now: %.24s\r\n", ctime(&ticks));
		write(connfd, buf, strlen(buf));
		close(connfd);
	}
}