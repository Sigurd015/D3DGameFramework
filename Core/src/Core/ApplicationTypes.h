#pragma once

#include "Application.h"

struct Application
{
    ApplicationSpecification Spec;

    void(*Ininialize)(Application* appInst);
    void(*Shutdown)(Application* appInst);
    void(*Update)(float timeStep);
};