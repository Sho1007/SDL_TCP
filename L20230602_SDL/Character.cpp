#include "Character.h"
#include <stdio.h>

ACharacter::ACharacter()
{
	Index = 0;
	Frame = 0;
	CharacterLocation = { 0,0 };
}

ACharacter::~ACharacter()
{
}

void ACharacter::Init(SDL_Renderer* TargetRenderer)
{
	// Load Surface
	SDL_Surface* CharacterSurface[3];
	for (int i = 0; i < 3; ++i)
	{
		char Buffer[1024] = { 0, };
		sprintf_s(Buffer, "Data/Stand_L_0%d.bmp", (i + 1));
		CharacterSurface[i] = SDL_LoadBMP(Buffer);
		CharacterTextureSize[i] = FPoint(CharacterSurface[i]->w, CharacterSurface[i]->h);
		SDL_SetColorKey(CharacterSurface[i], 1, SDL_MapRGB(CharacterSurface[i]->format, 255, 255, 255));
		CharacterTexture[i] = SDL_CreateTextureFromSurface(TargetRenderer, CharacterSurface[i]);
	}

	for (int i = 0; i < 3; ++i)
	{
		SDL_FreeSurface(CharacterSurface[i]);
	}
}

void ACharacter::Update()
{
	if (++Frame >= 60)
	{
		Frame = 0;
		Index = (Index + 1) % 3;
	}
	CharacterRect = {CharacterLocation.X, CharacterLocation.Y, CharacterTextureSize[Index].X, CharacterTextureSize[Index].Y};
}

void ACharacter::Render(SDL_Renderer* TargetRenderer)
{
	SDL_RenderCopy(TargetRenderer, CharacterTexture[Index], nullptr, &CharacterRect);
}

void ACharacter::Destroy()
{
}

void ACharacter::SetCharacterLocation(FPoint NewLocation)
{
	CharacterLocation = NewLocation;
}
