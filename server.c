#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#define BUFSZ 512

void logexit(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

unsigned long long int factorial(int n) {
    int result = 1, i;
    for (i = 2; i <= n; i++)
        result *= i;
    return result;
}

unsigned long long int fib(int n) {
   if (n <= 1)
      return n;
   return fib(n-1) + fib(n-2);
}

void processRequest(char* msg, int bufSize, int *exit, int *close) {

    char operation[50];
    int number = -1;

    sscanf(msg, "%s %d", operation, &number);

    if(strcmp(operation, "exit") == 0) {
 		*exit = 1;
    } else if (strcmp(operation, "close") == 0) {
    	*exit = 1;
    	*close = 1;
    } else {
		if(strcmp(operation, "fat") == 0) {
			if (number < 0)	strncpy(msg, "\nNumber must be positive", bufSize);
		    else sprintf(msg, "\n%llu", factorial(number));
		} else if(strcmp(operation, "fib") == 0) {
			if (number < 0)	strncpy(msg, "\nnumber must be positive", bufSize);
			else sprintf(msg, "\n%llu", fib(number));
		} else {
			strncpy(msg, "Invalid operation", bufSize);
		}  
	}	
}

int server_sockaddr_init(const char *proto, const char *portstr,
                         struct sockaddr_storage *storage) {
    uint16_t port = (uint16_t)atoi(portstr); // unsigned short
    if (port == 0) {
        return -1;
    }
    port = htons(port); // host to network short

    memset(storage, 0, sizeof(*storage));
    if (0 == strcmp(proto, "v4")) {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_addr.s_addr = INADDR_ANY;
        addr4->sin_port = port;
        return 0;
    } else if (0 == strcmp(proto, "v6")) {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_addr = in6addr_any;
        addr6->sin6_port = port;
        return 0;
    } else {
        return -1;
    }
}

void usage(int argc, char **argv) {
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage)) {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    }

    int enable = 1;
    if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        logexit("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage))) {
        logexit("bind");
    }

    if (0 != listen(s, 10)) {
        logexit("listen");
    }

	int closeSock = 0;

    while (!closeSock) {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        //socket de conexao com o cliente
        int csock = accept(s, caddr, &caddrlen);

        //testa se houve erro de conexao
        if (csock == -1) {
            logexit("accept");
        }

        char buf[BUFSZ];
		int exitSock = 0;

		while(!exitSock) {
			memset(buf, 0, BUFSZ);
			size_t count = recv(csock, buf, BUFSZ - 1, 0);

			processRequest(buf, sizeof(buf), &exitSock, &closeSock);
			printf("\n%s\n", buf);

			if (!exitSock) {
				count = send(csock, buf, strlen(buf) + 1, 0);
				if (count != strlen(buf) + 1) {
					logexit("send");
				}	
			} else {
				close(csock);
			}
		}
    }

    exit(EXIT_SUCCESS);
}
