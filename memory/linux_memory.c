#include "../include/memory.h"
#include "stdbool.h"
#include "stdint.h"
#include <sys/mman.h>

inline void *platform_memory_allocate_block(size_t reserved_size,
                                            size_t alignment) {
  return mmap(NULL, _align_size_to(reserved_size, alignment), PROT_NONE,
              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

inline size_t platform_memory_default_page_size() {
  return (size_t)sysconf(_SC_PAGESIZE);
}

inline bool platform_memory_check_to_commit_protect(void *block,
                                                    size_t to_commit) {
  return (mprotect(block, to_commit, PROT_READ | PROT_WRITE) == 0)
}
