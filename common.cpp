#include "common.h"


bool initWsa(int errVal){
    if (errVal != 0) {
		std::cerr << "Error WinSock version initializaion:";
		std::cerr << WSAGetLastError();
		return FALSE;
	}
	else {
		std::cout << "WinSock initialization is OK" << std::endl;
        return TRUE;
    }
}

bool ipTrans(int errVal){
    if (errVal == 0) {
		std::cerr << "Your IP string is not a valid IPv4/IPv6 string" << std::endl;
		return FALSE;
	}
    else if(errVal == -1){
        std::cerr << "occured problem in IP translation:";
        std::cerr << WSAGetLastError;
        return FALSE;
    }
    else{
        return TRUE;
    }
}

bool checkSock(SOCKET sk){ // TODO: добавить отдельный вывод при приеме серверного сокета и сокета клиента
    if (sk == INVALID_SOCKET) {
		std::cerr << "Error initialization socket # " << WSAGetLastError() << std::endl;
		return FALSE;
	}
	else {
		std::cout << "Client socket initialization is OK" << std::endl;
        return TRUE;
    }
}

bool sockOperSuccess(int oper, int errVal){
    switch(oper){
        case 0:
            if (errVal != 0) {
		        std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
		        return FALSE;
	        }
	        else {
		        std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server" << std::endl;
                return TRUE;
            }
        case 1:    
            if(errVal != 0){
                std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
                return FALSE;
            }
            else{
                std::cout << "Binding socket to Server info is OK" << std::endl;
                return TRUE;
            }
        case 2:
            if(errVal == -1){
                std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
                return FALSE;
            }
            else{
                return TRUE;
            }
        case 3:
            if(errVal == -1){
                std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
                return FALSE;
            }
            else{
                return TRUE;
            }
        case 4: // ! операция ACCEPT работает с SOCKET переписать это
            if(errVal == 0){
                std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
                return FALSE;
            }
            else{
                return TRUE;
            }
        case 5:
            if(errVal != 0){
                std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
                return FALSE;
            }
            else{
                std::cout << "Listening..." << std::endl;
                return TRUE;
            }
        default:
            std::cout << "no such operation" << std::endl;
            return FALSE;    
    }
}