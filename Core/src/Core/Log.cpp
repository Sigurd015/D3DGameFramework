#include "pch.h"
#include "Log.h"

FILE* LogFile = nullptr;

void Log_Initialize()
{
#ifdef BV_DIST
	LogFile = fopen("log.txt", "w");

	BV_ASSERT(LogFile, "Open log file failed");
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
		printf("\033[32m>%s-[ERROR]: %s\n\033[0m", type, message);
		break;
	}
}

void Log_Output_To_File(Log_Level level, const char* type, const char* message)
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
			fprintf(LogFile, ">%s-[ERROR]: %s\n", type, message);
			break;
		}
	}
}

void Log_Core_Output(Log_Level level, const char* message)
{
#ifdef BV_DIST
	Log_Output_To_File(level, "[CORE]", message);
#else
	Log_Output(level, "[CORE]", message);
#endif
}

void Log_APP_Output(Log_Level level, const char* message)
{
#ifdef BV_DIST
	Log_Output_To_File(level, "[APP]", message);
#else
	Log_Output(level, "[APP]", message);
#endif
}