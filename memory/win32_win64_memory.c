#include "../include/memory.h"
#include "stdbool.h"
#include "stdint.h"
#include <windows.h>

inline void *platform_memory_allocate_block(size_t reserved_size,
                                            size_t alignment) {
  return VirtualAlloc(NULL, _align_size_to(reserved_size, alignment),
                      MEM_RESERVE, PAGE_NOACCESS);
}

inline size_t platform_memory_default_page_size() {
  SYSTEM_INFO sys_info;
  GetSystemInfo(&sys_info);
  return (size_t)sys_info.dwPageSize;
}

inline bool platform_memory_check_to_commit_protect(void *block,
                                                    size_t to_commit) {
  return VirtualAlloc(block, to_commit, MEM_COMMIT, PAGE_READWRITE) != NULL;
}
