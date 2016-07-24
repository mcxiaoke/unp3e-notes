#include <stdio.h>
#include <unp.h>
#include "sockaddr.h"
#include "tcp.h"

int main(int argc, char *argv[]) {
	int listenfd, connfd;
	socklen_t len, addrlen;
	char buf[MAXLINE];
	time_t ticks;
	struct sockaddr_storage client_addr;
	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		err_quit("usage: cmd [host] service/port");
	}
	for (;;) {
		len = sizeof(client_addr);
		connfd = accept(listenfd, (struct sockaddr *)&client_addr, &len);
		printf("connection from %s\n", sockaddr_ntop((struct sockaddr *)&client_addr));
		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
		write(connfd, buf, strlen(buf));
		close(connfd);
	}
	return 0;
}