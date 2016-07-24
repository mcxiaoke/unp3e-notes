#include <stdio.h>
#include <unp.h>
#include "sockaddr.h"


int main(int argc, char *argv[]) {
	const char* hostname = argv[1];
	const char* port = "80";
	char buf[INET6_ADDRSTRLEN];
	struct addrinfo hints, *res, *tres;
	struct sockaddr_in *addr;
	struct sockaddr_in6 *addr6;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	int status = getaddrinfo(hostname, port, &hints, &res);
	if (status != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(status));
		exit(status);
	}
	int i = 0;
	short p = 0;
	for (tres = res; tres != NULL; tres = tres->ai_next) {
		char* ip = sockaddr_ntop(tres->ai_addr);
		printf("%s\n", ip);
		free(ip);
	}
	freeaddrinfo(res);
	return 0;
}