#pragma once

#include <iostream>
#include <WinSock2.h>
#include <map>
#include <string.h>

// * defines for easier usage of sockOperSuccess func
#define CONNECT 0
#define BIND 1
#define SEND 2
#define RECV 3
#define ACCEPT 4
#define LISTEN 5

bool initWsa(int errVal);  // проверка успешности инициализации WSADATA
bool ipTrans(int errVal); // проверка успешности преобразования строки IP адреса
bool checkSock(SOCKET sk); // проверка успешности создания сокета Сервера/Клиента
bool sockOperSuccess(int oper, int errVal); // проверка успешности операций