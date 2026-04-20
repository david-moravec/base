#include <stdlib.h>
#include <string.h>

#include "list.h"

#define DEFAULT_INIT_SIZE 32
#define SIZE_MULTIPLIER 2

typedef unsigned char byte;

static void *_get(void *base, size_t el_size, size_t index) {
  return &((byte *)base)[index * el_size];
}

static void _list_shift_left(void *data, size_t count, size_t element_size,
                             size_t index) {
  for (size_t i = index; i <= count; i++) {
    memcpy(_list_get(data, count, element_size, i),
           _list_get(data, count, element_size, i + 1), element_size);
  }
}

static size_t _grow_list(void **data, size_t size, Arena *arena) {
  if (arena == NULL) {
    return size;
  }

  if (data == NULL) {
    size = size == 0 ? DEFAULT_INIT_SIZE : size;
    *data = arena_push(arena, size);

    return size;
  } else {
    size_t new_size =
        size <= DEFAULT_INIT_SIZE ? DEFAULT_INIT_SIZE : size * SIZE_MULTIPLIER;
    *data = arena_realloc(arena, data, size, new_size);

    return new_size;
  }
};

void _list_init(void *data) { data = NULL; }

void *_list_get(void *data, size_t count, size_t element_size, size_t index) {
  if (index >= count) {
    return NULL;
  }

  return _get(data, element_size, index);
}

void *_list_tail(void *data, size_t count, size_t element_size) {
  return _list_get(data, count, element_size, count - 1);
}
void *_list_head(void *data, size_t count, size_t element_size) {
  return _list_get(data, count, element_size, 0);
}

void *_list_append(void **data, size_t *size, size_t *count,
                   size_t element_size, Arena *arena) {
  if (size == NULL || count == NULL) {
    return NULL;
  }

  if (*count == *size) {
    size_t new_size = _grow_list(data, *size, arena);

    if (new_size == *size) {
      return NULL;
    }

    *size = new_size;
  }

  (*count)++;

  return _list_tail(*data, *count, element_size);
}

void *_list_push(void **data, size_t size, size_t *count, size_t element_size,
                 void *element, Arena *arena) {
  void *new_entry = _list_append(data, &size, count, element_size, arena);

  if (new_entry == NULL) {
    return NULL;
  }

  memcpy(new_entry, element, element_size);
  return new_entry;
}

void *_list_remove(void *data, size_t *count, size_t element_size,
                   size_t index) {
  void *dest = _list_get(data, *count, element_size, index);
  void *result = dest;
  _list_shift_left(data, *count, element_size, index);
  (*count)--;

  return result;
}

void *_list_pop(void *data, size_t *count, size_t element_size) {
  void *result = _list_tail(data, *count, element_size);
  (*count)--;

  return result;
}
