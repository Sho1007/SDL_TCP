#pragma once
#include "SDL.h"
#include "PCH.h"


class ACharacter
{
public:
	ACharacter();
	virtual ~ACharacter();
public:
	void Init(SDL_Renderer* TargetRenderer);
	void Update();
	void Render(SDL_Renderer* TargetRenderer);
	void Destroy();

public:
	// Getter / Setter
	void SetCharacterLocation(FPoint NewLocation);

private:
	SDL_Rect CharacterRect;

	SDL_Texture* CharacterTexture[3];
	FPoint CharacterTextureSize[3];

	FPoint CharacterLocation;

	int Index;
	int Frame;
};