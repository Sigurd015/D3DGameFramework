#include "pch.h"
#include "Audio.h"

#include <Audio.h>
#include <atlbase.h>
#include <atlconv.h>

struct AudioData
{
	DirectX::AudioEngine* AudioEngine;
};
static AudioData s_AudioData;

void Audio_Init()
{
	s_AudioData.AudioEngine = new DirectX::AudioEngine();
}

void Audio_Update()
{
	s_AudioData.AudioEngine->Update();
}

void Audio_Shutdown()
{
	s_AudioData.AudioEngine->Suspend();
	delete s_AudioData.AudioEngine;
	s_AudioData.AudioEngine = nullptr;
}

void Audio_Suspend()
{
	s_AudioData.AudioEngine->Suspend();
}

void Audio_Resume()
{
	s_AudioData.AudioEngine->Resume();
}

void* Audio_CreateSoundEffect(const char* path)
{
	char vertexShaderName[256];
	sprintf_s(vertexShaderName, 256, "%s", path);
	DirectX::SoundEffect* soundEffect = new DirectX::SoundEffect(s_AudioData.AudioEngine, CA2T(path));
	return soundEffect;
}