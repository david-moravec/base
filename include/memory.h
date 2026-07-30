#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

#include "stdbool.h"
#include "stdint.h"

void *platform_memory_allocate_block(size_t reserved_size, size_t alignment);
size_t platform_memory_default_page_size();
bool platform_memory_check_to_commit_protect(void *block, size_t to_commit);

inline static size_t _align_size_to(size_t size, size_t alignment) {
  return (size + alignment - 1) / alignment * alignment;
}

#endif
