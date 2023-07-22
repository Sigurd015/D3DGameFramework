#pragma once

#ifdef BV_DEBUG
#define BV_ASSERT(x,m) {if(!(x)){CORE_LOG_ERROR(m);__debugbreak();}}
#else
#define BV_ASSERT(x,m) {if(!(x)){CORE_LOG_ERROR(m);}}
#endif

#define BIT(x) (1 << x)