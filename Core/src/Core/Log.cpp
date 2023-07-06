#include "pch.h"
#include "Log.h"

void Log_Initialize()
{
	// TODO:
}

void Log_Shutdown()
{
	// TODO:
}

void Log_Output(Log_Level level, const char* type, const char* message)
{
	switch (level)
	{
	case LOG_LEVEL_ERROR:
		printf("\033[31m>%s-[ERROR]: %s\n", type, message);
		break;
	case LOG_LEVEL_WARN:
		printf("\033[33m>%s-[WARN]: %s\n", type, message);
		break;
	case LOG_LEVEL_INFO:
		printf("\033[32m>%s-[ERROR]: %s\n", type, message);
		break;
	}
}

void Log_Core_Output(Log_Level level, const char* message)
{
	Log_Output(level, "[CORE]", message);
}

void Log_APP_Output(Log_Level level, const char* message)
{
	Log_Output(level, "[APP]", message);
}