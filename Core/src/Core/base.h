#pragma once

#ifdef CORE_DEBUG
#define CORE_ASSERT(x,m) {if(!(x)){CORE_LOG_ERROR(m);__debugbreak();}}
#else
#define CORE_ASSERT(x,m) {if(!(x)){CORE_LOG_ERROR(m);}}
#endif

#define BIT(x) (1 << x)