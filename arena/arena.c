#include "../include/arena.h"
#include <memory.h>
#include <stdlib.h>
#include <string.h>

#include "../include/memory.h"

#include <stddef.h>
#include <unistd.h>

size_t page_size(void) {
  static size_t size = 0;
  if (size == 0) {
    size = platform_memory_default_page_size();
  }
  return size;
}

Arena *arena_alloc(size_t reserved_size, size_t alignment) {
  Arena *arena = (Arena *)malloc(sizeof(Arena));
  if (alignment == 0) {
    arena->alignment = page_size();
  } else {
    arena->alignment = alignment;
  }

  void *block = platform_memory_allocate_block(reserved_size, arena->alignment);
  arena->reserved_size = reserved_size;
  arena->commited_size = 0;
  arena->pos = 0;
  arena->base_ptr = block;

  return arena;
}

void arena_release(Arena *arena) {
  if (arena != NULL) {
    if (arena->base_ptr != NULL) {
      free(arena->base_ptr);
    }
    arena->base_ptr = NULL;
    free(arena);
  }
  arena = NULL;
}

void *arena_push_no_zero(Arena *arena, size_t size) {
  if (arena == NULL || size == 0) {
    return NULL;
  }

  size_t size_aligned = _align_size_to(size, arena->alignment);
  size_t new_pos = arena->pos + size_aligned;

  if (new_pos > arena->reserved_size) {
    return NULL;
  }

  if (new_pos > arena->commited_size) {
    size_t to_commit = new_pos - arena->commited_size;
    if (!platform_memory_check_to_commit_protect(
            arena->base_ptr + arena->commited_size, to_commit)) {
      return NULL;
    }
    arena->commited_size += to_commit;
  }
  void *memory = arena->base_ptr + arena->pos;
  arena->pos += size;

  return memory;
}

void *arena_push(Arena *arena, size_t size) {
  void *mem = arena_push_no_zero(arena, size);
  memset(mem, 0, size);

  return mem;
}

void *arena_realloc(Arena *arena, void *ptr, size_t old_size, size_t new_size) {
  return arena_push(arena, new_size);
}

void arena_pop(Arena *arena, size_t size) { arena->pos -= size; }
void arena_clear(Arena *arena) { arena->pos = 0; }
