#include <stdlib.h>
#include <string.h>

#include "list.h"

#define DEFAULT_INIT_SIZE 32
#define SIZE_MULTIPLIER 2

typedef unsigned char byte;

void list_init(struct list *list, size_t el_size, size_t init_size) {
  list->size = init_size > 0 ? init_size : DEFAULT_INIT_SIZE;
  list->el_size = el_size;
  list->count = 0;
  list->data = (void *)malloc(list->el_size * list->size);
  memset(list->data, 0, list->el_size * list->size);
}

void list_free(struct list *list) {
  free(list->data);
  list->data = NULL;
  list->el_size = 0;
  list->size = 0;
}

static void *_get(void *base, size_t el_size, size_t index) {
  return &((byte *)base)[index * el_size];
}

void *list_get(struct list *list, size_t index) {
  if (index >= list->count) {
    return NULL;
  }

  return _get(list->data, list->el_size, index);
}

void *list_tail(struct list *list) { return list_get(list, list->count); }
void *list_head(struct list *list) { return list_get(list, 0); }

void list_grow(struct list *list) {
  size_t old_size = list->size;
  size_t new_size = SIZE_MULTIPLIER * old_size;
  list->data = realloc(list->data, new_size * list->el_size);
  list->size = new_size;
}

void check_grow_list(struct list *list) {
  if (list->count == list->size) {
    list_grow(list);
  }
}

void *list_append(struct list *list, size_t *index) {
  check_grow_list(list);

  if (index != NULL) {
    *index = list->count;
  }

  list->count++;

  return list_get(list, list->count - 1);
}

void *list_append_copy(struct list *list, void *element, size_t *index) {
  void *dest = list_append(list, index);
  memcpy(dest, element, list->el_size);
  return dest;
}

void *list_append_many(struct list *list, void *elements, size_t len_elements,
                       size_t *end_index) {

  void *final_dest;

  for (size_t i = 0; i < len_elements; i++) {
    void *final_dest = list_append(list, end_index);
    final_dest = &elements[i * list->el_size];
  }

  return final_dest;
}

void *list_insert_copy(struct list *list, void *element, size_t index) {
  if (index > list->count) {
    return NULL;
  }

  void *dest = list_get(list, index);
  memcpy(dest, element, list->el_size);
  return dest;
}

// void list_shift_up(struct list *list, size_t index) {
//   size_t orig_size = list->size;

//   check_grow_list(list);

//   for (size_t i = orig_size; i >= index; i--) {
//     memcpy(list_get(list, i + 1), list_get(list, i), list->el_size);
//   }
// }

void *list_remove(struct list *list, size_t index) {
  void *dest = list_get(list, index);
  void *result = dest;
  dest = NULL;
  return result;
}
