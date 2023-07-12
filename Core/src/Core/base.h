#pragma once

#ifdef BV_DEBUG
#define BV_ASSERT(x,m) {if(!x){__debugbreak();CORE_LOG_ERROR(m);}}
#else
#define BV_ASSERT(x,m) x
#endif