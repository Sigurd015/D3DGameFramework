#include "pch.h"
#include "Log.h"

#include <cstdarg>

#define LOG_BUFFER_SIZE 256

FILE* LogFile = nullptr;

void Log_Initialize()
{
	#ifdef CORE_DIST
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

void Output(Log_Level level, const char* type, const char* message)
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

void OutputToFile(Log_Level level, const char* type, const char* message)
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

void Log_Output(Log_Level level, const char* type, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char logMessage[LOG_BUFFER_SIZE];
	vsnprintf(logMessage, sizeof(logMessage), format, args);
	va_end(args);

	#ifdef CORE_DIST
	OutputToFile(level, type, logMessage);
	#else
	Output(level, type, logMessage);
	#endif
}