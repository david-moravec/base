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

void *_list_get(void *data, size_t count, size_t element_size, size_t index) {
  if (index >= count) {
    return NULL;
  }

  return _get(data, element_size, index);
}

void *_list_tail(void *data, size_t count, size_t element_size) {
  return _list_get(data, count, element_size, count);
}
void *_list_head(void *data, size_t count, size_t element_size) {
  return _list_get(data, count, element_size, 0);
}

void *_list_append(void *data, size_t size, size_t *count, size_t element_size,
                   size_t *index) {
  if (*count == size) {
    return NULL;
  }

  if (index != NULL) {
    *index = *count;
  }

  (*count)++;

  return _list_tail(data, (*count) - 1, element_size);
}

void *_list_push(void *data, size_t size, size_t *count, size_t element_size,
                 void *element, size_t *index) {
  void *new_entry = _list_append(data, size, count, element_size, index);

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
