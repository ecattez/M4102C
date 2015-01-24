#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"

int main(int argc, char **argv)
{
	int port;
	int socket_client;
	char buf[256];
	
	if (argc < 2) {
		printf("Usage: /l7pserv [port]\n");
		return -1;
	}
	
	port = atoi(argv[1]);
	
	if (port == 0) {
		printf("Port must be an int > 0 !\n");
		return -1;
	}
	
	socket_client = creer_serveur(port);

	while(socket_client != -1)
	{
		/* On lit le buffer */
		if (read(socket_client, &buf, sizeof(buf)-1) != -1)
		{
			/* On écrit le message de l'utilisateur à l'utilisateur (echo) */
			write(socket_client, &buf, strlen(buf));
			
			/* On écrit le message au serveur pour savoir quel client à écrit quoi */
			printf("Client(id=%d) send: %s", socket_client, buf);
			
			/* Le client quitte le serveur s'il écrit le message voulu */
			if (strncmp(buf, "quit", 4) == 0)
			{
				close(socket_client);
				return 0;
			}
			
			/* On efface le buffer pour ne pas avoir de caractères résiduels */
			memset(&buf,0,255);
		}
	}
	return 0;
}
