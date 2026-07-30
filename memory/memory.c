#include "../include/memory.h"
#include "stdbool.h"
#include "stdint.h"
#if defined(WIN32) || defined(WIN64)
#include "../memory/win32_win64_memory.c"
#else
#include "../memory/linux_memory.c"
#endif
