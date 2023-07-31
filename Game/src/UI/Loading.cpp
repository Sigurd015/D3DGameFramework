#include "Core.h"
#include "Loading.h"
#include "Common.h"

struct StartUpData
{
	Canvas Canvas;

	GameMode Depature = STARTUP_TITLE;
};
static StartUpData s_Data;

void Loading_Initialize()
{
	s_Data.Depature = STARTUP_TITLE;
}

void Loading_Draw(float timeStep)
{

}

void Loading_SetDepature(GameMode depature)
{
	s_Data.Depature = depature;
}

void Loading_Destroy()
{

}