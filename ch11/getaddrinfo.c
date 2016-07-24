#include <stdio.h>
#include <unp.h>

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
		switch (tres->ai_family) {
			case AF_INET:
				addr = (struct sockaddr_in *) tres->ai_addr;
				p = ntohs(addr->sin_port);
				inet_ntop(AF_INET, &addr->sin_addr, 
					buf, INET_ADDRSTRLEN);
				printf("%s\n", buf);
				break;
			case AF_INET6:
				addr6 = (struct sockaddr_in6 *) tres->ai_addr;
				p = ntohs(addr6->sin6_port);
				inet_ntop(AF_INET6, &addr6->sin6_addr, 
					buf, INET6_ADDRSTRLEN);
				printf("%s\n", buf);
				break;
			default:
				printf("unknown family: %d", tres->ai_family);
				break;
		}
	}
	freeaddrinfo(res);
	return 0;
}