#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint8_t *base_ptr;
  size_t pos;
  size_t reserved_size;
  size_t commited_size;
  size_t alignment;
} Arena;

Arena *arena_alloc(size_t resereved_size, size_t alignment);
void arena_release(Arena *arena);

void *arena_push(Arena *arena, size_t size);
void *arena_push_zero(Arena *arena, size_t size);

void arena_pop(Arena *arena, size_t size);
void arena_clear(Arena *arena);

inline size_t arena_get_pos(Arena *arena) { return arena->pos; };
