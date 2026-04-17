#include "arena.h"
#include <stdlib.h>
#include <sys/mman.h>

#include <stddef.h>
#include <unistd.h>

size_t page_size(void) {
  static size_t size = 0;
  if (size == 0) {
    size = (size_t)sysconf(_SC_PAGESIZE);
  }
  return size;
}

static size_t _align_size_to(size_t size, size_t alignment) {
  return (size + alignment - 1) / alignment * alignment;
}

Arena *arena_alloc(size_t reserved_size, size_t alignment) {
  Arena *arena = (Arena *)malloc(sizeof(Arena));
  if (alignment == 0) {
    arena->alignment = page_size();
  } else {
    arena->alignment = alignment;
  }

  void *block = mmap(NULL, _align_size_to(reserved_size, arena->alignment),
                     PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  arena->reserved_size = reserved_size;
  arena->commited_size = 0;
  arena->pos = 0;

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

void *arena_push(Arena *arena, size_t size) {
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
    if (mprotect(arena->base_ptr + arena->commited_size, to_commit,
                 PROT_READ | PROT_WRITE) != 0) {
      return NULL;
    }

    arena->commited_size += to_commit;
  }

  void *memory = arena->base_ptr + arena->pos;
  arena->pos += size;

  return memory;
}

void *arena_push_zero(Arena *arena, size_t size) {
  arena_clear(arena);
  return arena_push(arena, size);
}

void arena_pop(Arena *arena, size_t size) { arena->pos -= size; }
void arena_clear(Arena *arena) { arena->pos = 0; }
