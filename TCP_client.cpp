// Client part for Server-Client chat. Developed by Mr_Dezz
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0600

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
//#include <inaddr.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

#include "common.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


// TODO: переписать работу с клиентом, создать его как класс

int main(void)
{
	//Key constants
	const char *SERVER_IP = "127.0.0.1";	// IPv4 address of Server
	const short SERVER_PORT_NUM = 55;				// Listening port on Server side
	const short BUFF_SIZE = 1024;					// Maximum size of buffer for exchange info between server and client

	// Key variables for all program
	int erStat;										// checking errors in sockets functions

	//IP in string format to numeric format for socket functions. Data is in "ip_to_num"
	in_addr ip_to_num;		
	erStat = inet_pton(AF_INET, SERVER_IP, &ip_to_num);

    if(!ipTrans(erStat)){
        return 1;
    }

	// WinSock initialization
	WSADATA wsData;
	erStat = WSAStartup(MAKEWORD(2,2), &wsData);

	if(!initWsa(erStat)){
        return 1;
    }
	
	// Socket initialization
	SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

    if(!checkSock(ClientSock)){
        closesocket(ClientSock);
		WSACleanup();
        return 1;
    }

	// Establishing a connection to Server
	sockaddr_in servInfo;

	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;	
	servInfo.sin_port = htons(SERVER_PORT_NUM);

	erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));
	
	if(!sockOperSuccess(CONNECT,erStat)){
		closesocket(ClientSock);
		WSACleanup();
		return 1;
	}


	//Exchange text data between Server and Client. Disconnection if a Client send "xxx"

	std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);							// Buffers for sending and receiving data
	short packet_size = 0;												// The size of sending / receiving packet in bytes
		
	while (true) {

		std::cout << "Your (Client) message to Server: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin); // TODO: убрать fgets и заменить другой функцией(к примеру getline)

		// Check whether client like to stop chatting 
		if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
			shutdown(ClientSock, SD_BOTH);
			closesocket(ClientSock);
			WSACleanup();
			return 0;
		}

		packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);

		if (!sockOperSuccess(SEND,packet_size)) {
			closesocket(ClientSock);
			WSACleanup();
			return 1;
		} 

		packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);

		if (!sockOperSuccess(RECV, packet_size)) {
			closesocket(ClientSock);
			WSACleanup();
			return 1;
		}
		else
			std::cout << "Server message: " << servBuff.data() << std::endl;

	}

	closesocket(ClientSock);
	WSACleanup();

	return 0;
}