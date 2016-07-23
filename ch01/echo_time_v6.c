#include <stdio.h>
#include <unp.h>

// http://beej.us/guide/bgnet/output/html/multipage/sockaddr_inman.html
int main(int argc, char *argv[]) {
	int sockfd, n;
	char recvline[MAXLINE+1];
	struct sockaddr_in6 servaddr;
	// ntp to 131.107.13.100
	char *ip = "0:0:0:0:0:ffff:836b:d64";
	printf("create socket to %s\n",ip);
	sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (sockfd < 0) {
		err_sys("socket error");
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_port = htons(13);
	if (inet_pton(AF_INET6, ip, &servaddr.sin6_addr) <= 0){
		err_quit("inet_pton error for %s\n", ip);
	}
	printf("connecting to %s\n",ip);
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		err_sys("connect error = %d", errno);
	}
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;
		if (fputs(recvline, stdout) == EOF) {
			err_sys("fputs error");
		}
	}
	if (n < 0) {
		err_sys("read error");
	}
	
	close(sockfd);
	return 0;
}
