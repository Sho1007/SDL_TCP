#include <winsock2.h>
#include <process.h>
#include <map>
#pragma comment (lib, "ws2_32")
using namespace std;

typedef struct _FPoint
{
	_FPoint(int NewX = 0, int NewY = 0) : X(NewX), Y(NewY) {}

	int X, Y;
} FPoint;

typedef struct _FCharacterInfo
{
	_FCharacterInfo(int NewPlayerID, FPoint NewPlayerLocation = FPoint()) : PlayerID(NewPlayerID), PlayerLocation(NewPlayerLocation) {}
	int PlayerID;
	FPoint PlayerLocation;
} FCharacterInfo;

typedef struct _FClientInput
{
	char InputKey;
} FClientInput;

map<SOCKET, FCharacterInfo> PlayerList;
CRITICAL_SECTION PlayerListCS;

UINT WINAPI ClientThread(void* Arg)
{
	SOCKET ClientSocket = *(SOCKET*)Arg;
	while (true)
	{
		FClientInput ClientInput;
		int RecvByte = recv(ClientSocket, (char*)&ClientInput, sizeof(ClientInput), 0);
		if (RecvByte <= 0)
		{
			EnterCriticalSection(&PlayerListCS);
			PlayerList.erase(ClientSocket);
			LeaveCriticalSection(&PlayerListCS);
		}
		else
		{
			// Todo : Do Something with input
			switch (ClientInput.InputKey)
			{
			case 'w':
				PlayerList[ClientSocket].PlayerLocation.Y -= 10;
				break;
			case 'a':
				PlayerList[ClientSocket].PlayerLocation.X -= 10;
				break;
			case 's':
				PlayerList[ClientSocket].PlayerLocation.Y += 10;
				break;
			case 'd':
				PlayerList[ClientSocket].PlayerLocation.X += 10;
				break;
			}
			EnterCriticalSection(&PlayerListCS);
			for (auto Player : PlayerList)
			{
				int SendByte = send(Player.first, (char*)&PlayerList[ClientSocket], sizeof(PlayerList[ClientSocket]), 0);
			}
			LeaveCriticalSection(&PlayerListCS);
		}
	}
	
	return 0;
}

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	InitializeCriticalSection(&PlayerListCS);

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN ListenSockArr;
	memset(&ListenSockArr, 0, sizeof(ListenSockArr));
	ListenSockArr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ListenSockArr.sin_family = PF_INET;
	ListenSockArr.sin_port = htons(33333);

	bind(ListenSocket, (sockaddr*)&ListenSockArr, sizeof(ListenSockArr));

	listen(ListenSocket, 0);

	while (1)
	{
		SOCKADDR_IN ClientSockAddr;
		int ClientSockAddrLength = sizeof(ClientSockAddr);
		memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
		SOCKET NewClientSocket = accept(ListenSocket, (sockaddr*)&ClientSockAddr, &ClientSockAddrLength);

		EnterCriticalSection(&PlayerListCS);
		PlayerList[NewClientSocket] = FCharacterInfo(NewClientSocket);
		int SendByte = send(NewClientSocket, (char*)&PlayerList[NewClientSocket], sizeof(PlayerList[NewClientSocket]), 0);
		LeaveCriticalSection(&PlayerListCS);

		HANDLE ThreadHandle = (HANDLE)_beginthreadex(0, 0, ClientThread, (void*)&NewClientSocket, 0, 0);
	}

	for (auto Player : PlayerList)
	{
		closesocket(Player.first);
	}

	DeleteCriticalSection(&PlayerListCS);

	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}