#pragma once

enum Log_Level
{
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN = 1,
    LOG_LEVEL_INFO = 2,
};

void Log_Initialize();
void Log_Shutdown();

void Log_Core_Output(Log_Level level, const char* format, ...);
void Log_APP_Output(Log_Level level, const char* format, ...);

#define CORE_LOG_ERROR(format, ...) Log_Core_Output(LOG_LEVEL_ERROR, format, ##__VA_ARGS__);
#define CORE_LOG_WARN(format, ...) Log_Core_Output(LOG_LEVEL_WARN, format, ##__VA_ARGS__);
#define CORE_LOG_INFO(format, ...) Log_Core_Output(LOG_LEVEL_INFO, format, ##__VA_ARGS__);

#define APP_LOG_ERROR(format, ...) Log_APP_Output(LOG_LEVEL_ERROR, format, ##__VA_ARGS__);
#define APP_LOG_WARN(format, ...) Log_APP_Output(LOG_LEVEL_WARN, format, ##__VA_ARGS__);
#define APP_LOG_INFO(format, ...) Log_APP_Output(LOG_LEVEL_INFO, format, ##__VA_ARGS__);