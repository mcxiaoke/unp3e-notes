#include <stdio.h>

#ifndef SOCKADDR_H
#define SOCKADDR_H

char* sockaddr_ntop(struct sockaddr *addr) {
	short p = 0;
	char *buf = NULL;
	struct sockaddr_in *addr4;
	struct sockaddr_in6 *addr6;
	switch (addr->sa_family) {
		case AF_INET:
			addr4 = (struct sockaddr_in *) addr;
			p = ntohs(addr4->sin_port);
			buf = malloc(sizeof(char) * INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &addr4->sin_addr, 
				buf, INET_ADDRSTRLEN);
//			printf("%s\n", buf);
			break;
		case AF_INET6:
			addr6 = (struct sockaddr_in6 *) addr;
			p = ntohs(addr6->sin6_port);
			buf = malloc(sizeof(char) * INET6_ADDRSTRLEN);
			inet_ntop(AF_INET6, &addr6->sin6_addr, 
				buf, INET6_ADDRSTRLEN);
//			printf("%s\n", buf);
			break;
		default:
			printf("unknown family %d\n", addr->sa_family);
			break;
	}
	return buf;
}

#endif