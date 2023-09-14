#pragma once

#ifdef CORE_DEBUG
#define CORE_ASSERT(x,format, ...) {if(!(x)){CORE_LOG_ERROR(format, ##__VA_ARGS__);__debugbreak();}}
#else
#define CORE_ASSERT(x,format, ...) {if(!(x)){CORE_LOG_ERROR(format, ##__VA_ARGS__);}}
#endif

#define BIT(x) (1 << x)