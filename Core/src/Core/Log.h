#pragma once

enum Log_Level
{
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN = 1,
    LOG_LEVEL_INFO = 2,
};

void Log_Initialize();
void Log_Shutdown();

void Log_Core_Output(Log_Level level, const char* message);
void Log_APP_Output(Log_Level level, const char* message);

#define CORE_LOG_ERROR(message) Log_Core_Output(LOG_LEVEL_ERROR, message);
#define CORE_LOG_WARN(message) Log_Core_Output(LOG_LEVEL_WARN, message);
#define CORE_LOG_INFO(message) Log_Core_Output(LOG_LEVEL_INFO, message);

#define APP_LOG_ERROR(message) Log_APP_Output(LOG_LEVEL_ERROR, message);
#define APP_LOG_WARN(message) Log_APP_Output(LOG_LEVEL_WARN, message);
#define APP_LOG_INFO(message) Log_APP_Output(LOG_LEVEL_INFO, message);