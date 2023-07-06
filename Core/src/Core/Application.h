#pragma once

#include <stdint.h>

struct Application;

struct ApplicationCommandLineArgs
{
    int Count = 0;
    char** Args = nullptr;
};

struct ApplicationSpecification
{
    char* Name = "DX2D App";
    ApplicationCommandLineArgs CommandLineArgs;
};

void Application_Create(Application* appInst);

void Application_Run();