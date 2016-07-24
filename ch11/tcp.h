#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <unp.h>
#include "sockaddr.h"

int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp){
	int listenfd, n;
	const int on = 1;
	struct addrinfo hints, *res, *ressave;
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
		err_quit("tcp listen error for %s:%s: %s", host, serv, gai_strerror(n));
	}
	ressave = res;
	do {
		listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (listenfd < 0) {
			continue;
		}
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0) {
			break;
		}
		close(listenfd);
	} while ((res = res->ai_next) != NULL);
	if (res == NULL) {
		err_sys("tcp listen error for %s:%s", host, serv);
	}
	listen(listenfd, LISTENQ);
	if (addrlenp) {
		*addrlenp = res->ai_addrlen;
	}
	freeaddrinfo(ressave);
	return listenfd;
}

int tcp_connect(const char *host, const char *serv){
	int sockfd, n;
	struct addrinfo hints, *res, *ressave;
	
	memset(&hints, 0, sizeof(hints));
//	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	printf("client = %s:%s\n", host, serv);
	if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
		err_quit("tcp connect error for %s:%s: %s",
			host, serv, gai_strerror(n));
	}
	ressave = res;
	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd < 0) {
			continue;
		}
		if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
			break;
		}
		close(sockfd);
	} while ((res = res->ai_next) != NULL);
	if (res == NULL) {
		err_sys("tcp connect error for %s, %s", host, serv);
	}
	freeaddrinfo(ressave);
	return sockfd;
}

#endif