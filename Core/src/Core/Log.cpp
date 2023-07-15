#include "pch.h"
#include "Log.h"

FILE* LogFile = nullptr;

void Log_Initialize()
{
#ifdef BV_DIST
	LogFile = fopen("log.txt", "w");
#endif 
}

void Log_Shutdown()
{
	if (LogFile != nullptr)
	{
		fclose(LogFile);
		LogFile = nullptr;
	}
}

void Log_Output(Log_Level level, const char* type, const char* message)
{
	switch (level)
	{
	case LOG_LEVEL_ERROR:
		printf("\033[31m>%s-[ERROR]: %s\n\033[0m", type, message);
		break;
	case LOG_LEVEL_WARN:
		printf("\033[33m>%s-[WARN]: %s\n\033[0m", type, message);
		break;
	case LOG_LEVEL_INFO:
		printf("\033[32m>%s-[INFO]: %s\n\033[0m", type, message);
		break;
	}
}

void Log_OutputToFile(Log_Level level, const char* type, const char* message)
{
	if (LogFile != nullptr)
	{
		switch (level)
		{
		case LOG_LEVEL_ERROR:
			fprintf(LogFile, ">%s-[ERROR]: %s\n", type, message);
			break;
		case LOG_LEVEL_WARN:
			fprintf(LogFile, ">%s-[WARN]: %s\n", type, message);
			break;
		case LOG_LEVEL_INFO:
			fprintf(LogFile, ">%s-[INFO]: %s\n", type, message);
			break;
		}
	}
}

void Log_Core_Output(Log_Level level, const char* message)
{
#ifdef BV_DIST
	Log_OutputToFile(level, "[CORE]", message);
#else
	Log_Output(level, "[CORE]", message);
#endif
}

void Log_APP_Output(Log_Level level, const char* message)
{
#ifdef BV_DIST
	Log_OutputToFile(level, "[APP]", message);
#else
	Log_Output(level, "[APP]", message);
#endif
}