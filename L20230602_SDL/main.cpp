#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#include <map>
#include "SDL.h"
#include "PCH.h"
#include "Character.h"
#pragma comment (lib, "ws2_32")
using namespace std;

map<int, ACharacter*> CharacterArray;

UINT WINAPI ReceiveFromServer(void* Arg)
{
	return 0;
}

int SDL_main(int argc, char* argv[])
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Server Connect

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN ServerSockAddr;
	int ServerSockAddrLength = sizeof(ServerSockAddr);
	memset(&ServerSockAddr, 0, ServerSockAddrLength);
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_port = htons(33333);

	connect(ServerSocket, (sockaddr*)&ServerSockAddr, ServerSockAddrLength);

	// Create My Character;
	FCharacterInfo CharacterInfo;
	int RecvByte = recv(ServerSocket, (char*)&CharacterInfo, sizeof(CharacterInfo), 0);
	CharacterArray[CharacterInfo.PlayerID] = new ACharacter();
	CharacterArray[CharacterInfo.PlayerID]->SetCharacterLocation(CharacterInfo.PlayerLocation);

	// SDL Init

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* MyWindow = SDL_CreateWindow("FirstWindow", 100, 100, 640, 480, SDL_WINDOW_OPENGL);
	SDL_Renderer* MyRenderer = SDL_CreateRenderer(MyWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Event MyEvent;
	bool bIsRunning = true;

	// Map Load

	SDL_Surface* MapSurface = SDL_LoadBMP("Data/_Map.bmp");
	SDL_Texture* MapTexture = SDL_CreateTextureFromSurface(MyRenderer, MapSurface);

	while (bIsRunning)
	{
		// Input
		SDL_PollEvent(&MyEvent);
		//Tick
		switch (MyEvent.type)
		{
		case SDL_QUIT:
			bIsRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (MyEvent.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				bIsRunning = false;
			case SDLK_UP:
				break;
			case SDLK_LEFT:
				break;
			case SDLK_DOWN:
				break;
			case SDLK_RIGHT:
				break;
			}
			break;
		}
		// Renderer Clear
		SDL_SetRenderDrawColor(MyRenderer, 0, 0, 0, 0);
		SDL_RenderClear(MyRenderer);

		// Draw Map
		SDL_Rect MapRect = { 0, 0, MapSurface->w, MapSurface->h };
		SDL_RenderCopy(MyRenderer, MapTexture, nullptr, &MapRect);

		// Draw Character
		for (auto Character : CharacterArray)
		{
			Character.second->Render(MyRenderer);
		}

		/*SDL_SetRenderDrawColor(MyRenderer, 207, 255, 229, 0);
		SDL_RenderFillRect(MyRenderer, &MyRect);*/

		// nullptr 로 넣으면 Texture 전체 크기
		//SDL_RenderCopy(MyRenderer, MyTexture[Index], nullptr, &MyRect);

		SDL_RenderPresent(MyRenderer);
	}

	for (auto Character : CharacterArray)
	{
		Character.second->Destroy();
		delete Character.second;
	}

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}