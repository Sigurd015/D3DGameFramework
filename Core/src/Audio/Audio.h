#pragma once

void Audio_Init();
void Audio_Update();
void Audio_Shutdown();
void Audio_Suspend();
void Audio_Resume();
void* Audio_CreateSoundEffect(const WCHAR* path);