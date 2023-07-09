#pragma once

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

void Application_Ininialize(Application* appInst);

void Application_Run();