#include "Renderer.h"
#include "../Common.h"

#include <stdlib.h>
#include <stdio.h>
#include <cstring>

struct RendererData
{
	char BackBuffer[SCREEN_BUFFER_SIZE];
};
RendererData s_Data;

void Renderer_UpdateData(char* data)
{
	strcpy(s_Data.BackBuffer, data);
}

void Renderer_Draw()
{
	system("cls");
	printf("%s", s_Data.BackBuffer);
	strcpy(s_Data.BackBuffer, "");
}