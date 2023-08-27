#pragma once
#include "Input/Keycode.h"
#include "Application.h"

struct Application
{
    ApplicationSpecification Spec;

    void(*Ininialize)(Application* appInst);
    void(*Shutdown)(Application* appInst);
    void(*Update)(float timeStep);
    void(*OnKeyPressed)(Application* appInst, KeyCode key);
};