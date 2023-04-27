
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0600


#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>

#include "common.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace std;


// ! ↓↓↓↓↓↓↓↓
// TODO: сделать нормальный MAKEFILE что бы 100500 раз не компилировать через консоль
// ! ↑↑↑↑↑↑↑↑

// TODO: переписать работу с сервером, создать его как класс
// TODO: переписать всеб что бы сервер мог работать в дуплекс режиме(развести прием и отправку сообщений по разным потокам)
// TODO: добавить дополнительные обработки ошибок подключения и тд. Что бы сервер сразу не завершал работу
int main(void)
{
	
	//Key constants
	const char *IP_SERV = "127.0.0.1";			// local Server IP address
	const int PORT_NUM = 55;				// Open server port
	const short BUFF_SIZE = 1024;			// Maximum size of buffer for exchange info between server and client

	// Key variables for all program
	int erStat;								// Keeps socket errors status

	//IP in string format to numeric format for socket functions. Data is in "ip_to_num"
	in_addr ip_to_num;
	erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);
	
	if(!ipTrans(erStat)){
        return 1;
    }

	// WinSock initialization
	WSADATA wsData;
		
	erStat = WSAStartup(MAKEWORD(2,2), &wsData);
	
	if(!initWsa(erStat)){
        return 1;
    }
	
	// Server socket initialization
	SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

	if(!checkSock(ServSock)){
        closesocket(ServSock);
		WSACleanup();
        return 1;
    }

	// Server socket binding
	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));	// Initializing servInfo structure
				
	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;	
	servInfo.sin_port = htons(PORT_NUM);

	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (!sockOperSuccess(BIND,erStat)) {
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}

	//Starting to listen to any Clients
	erStat = listen(ServSock, SOMAXCONN);

	if (!sockOperSuccess(LISTEN,erStat)) {
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}

	//Client socket creation and acception in case of connection
	sockaddr_in clientInfo; 
	ZeroMemory(&clientInfo, sizeof(clientInfo));	// Initializing clientInfo structure

	int clientInfo_size = sizeof(clientInfo);

	SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

	if(!checkSock(ServSock)){
        closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();
        return 1;
    }
	else {
		cout << "Connection to a client established successfully" << endl;
		char clientIP[22];

		inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);	// Convert connected client's IP to standard string format

		cout << "Client connected with IP address " << clientIP << endl;

	}

	//Exchange text data between Server and Client. Disconnection if a client send "xxx"

	vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);							// Creation of buffers for sending and receiving data
	short packet_size = 0;												// The size of sending / receiving packet in bytes
	
	while (true) {
		packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);	
		if (!sockOperSuccess(RECV, packet_size)) {
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 1;
		}
		else				// Receiving packet from client. Program is waiting (system pause) until receive
			cout << "Client's message: " << servBuff.data() << endl; 

		cout << "Your (host) message: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		// Check whether server would like to stop chatting 
		if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
			shutdown(ClientConn, SD_BOTH);
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 0;
		}

		packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);

		if (!sockOperSuccess(SEND,packet_size)) {
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 1;
		}

	}

	closesocket(ServSock);
	closesocket(ClientConn);
	WSACleanup();

	return 0;

}