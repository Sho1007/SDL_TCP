#pragma once

typedef struct _FPoint
{
	_FPoint(int NewX = 0, int NewY = 0) : X(NewX), Y(NewY) {}

	int X, Y;
} FPoint;

typedef struct _FCharacterInfo
{
	_FCharacterInfo(int NewPlayerID = 0, FPoint NewPlayerLocation = FPoint()) : PlayerID(NewPlayerID), PlayerLocation(NewPlayerLocation) {}
	int PlayerID;
	FPoint PlayerLocation;
} FCharacterInfo;

typedef struct _FClientInput
{
	char InputKey;
} FClientInput;