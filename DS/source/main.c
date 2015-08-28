#include <nds.h>
#include <dswifi9.h>
#include <netinet/in.h>

#include <stdio.h>

#define IP(a, b, c, d) ((a) + ((b) << 8) + ((c) << 16) + ((d) << 24))

int main(void) {
	consoleDemoInit();
	
	if(!Wifi_InitDefault(WFC_CONNECT)) {
		printf("Failed to connect!");
		return 0;
	}
	
	struct sockaddr_in server;
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 1, 2);
	server.sin_port = htons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	//int sock = socket(PF_INET, SOCK_STREAM, 0);
	//connect(sock, (struct sockaddr *)&server, sizeof(server));
	
	while(1) {
		int keys;
		scanKeys();
		keys = keysHeld();
		
		//send(sock, &keys, 4, 0);
		sendto(sock, &keys, 4, 0, (struct sockaddr *)&server, sizeof(server));
		
		swiWaitForVBlank();
	}
	
	closesocket(sock);

	return 0;
}
