#pragma once

enum Log_Level
{
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN = 1,
    LOG_LEVEL_INFO = 2,
};

void Log_Initialize();
void Log_Shutdown();

void Log_Output(Log_Level level, const char* type, const char* format, ...);

#define CORE_LOG_ERROR(format, ...) Log_Output(LOG_LEVEL_ERROR, "[CORE]", format, ##__VA_ARGS__);
#define CORE_LOG_WARN(format, ...) Log_Output(LOG_LEVEL_WARN, "[CORE]", format, ##__VA_ARGS__);
#define CORE_LOG_INFO(format, ...) Log_Output(LOG_LEVEL_INFO, "[CORE]", format, ##__VA_ARGS__);

#define APP_LOG_ERROR(format, ...) Log_Output(LOG_LEVEL_ERROR, "[APP]", format, ##__VA_ARGS__);
#define APP_LOG_WARN(format, ...) Log_Output(LOG_LEVEL_WARN, "[APP]", format, ##__VA_ARGS__);
#define APP_LOG_INFO(format, ...) Log_Output(LOG_LEVEL_INFO, "[APP]", format, ##__VA_ARGS__);