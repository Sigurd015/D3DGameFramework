#include "pch.h"
#include "Audio.h"

#include <Audio.h>

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
	size_t newsize = strlen(path) + 1;

	wchar_t* wcstring = new wchar_t[newsize];

	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, path, _TRUNCATE);

	DirectX::SoundEffect* soundEffect = new DirectX::SoundEffect(s_AudioData.AudioEngine, wcstring);

	delete[]wcstring;
	return soundEffect;
}