#ifndef MY_CUSTOM_LIST_H
#define MY_CUSTOM_LIST_H

#include <stdlib.h>

void *_list_append(void *data, size_t size, size_t *count, size_t element_size,
                   size_t *index);
void *_list_push(void *data, size_t size, size_t *count, size_t element_size,
                 void *element, size_t *index);
void *_list_remove(void *data, size_t *count, size_t element_size,
                   size_t index);
void *_list_pop(void *data, size_t *count, size_t element_size);

void *_list_get(void *data, size_t count, size_t index, size_t element_size);
void *_list_head(void *data, size_t count, size_t element_size);
void *_list_tail(void *data, size_t count, size_t element_size);

#define List(type)                                                             \
  struct {                                                                     \
    size_t size;                                                               \
    size_t count;                                                              \
    size_t element_size;                                                       \
    type *data;                                                                \
  }

#define list_append(list)                                                      \
  (_list_append((list).data, (list).size, &(list).count, (list).element_size,  \
                NULL))

#define list_push(list, element)                                               \
  (_list_push((list).data, (list).size, &(list).count, (list).element_size,    \
              1 ? (element) : (list).data, NULL))

#define list_remove(list, index)                                               \
  (typeof((list).data))_list_remove((list).data, &(list).count,                \
                                    (list).element_size, (index))

#define list_pop(list)                                                         \
  (typeof((list).data))(_list_pop((list).data, &(list).count, (list).element_size)

#define list_get(list, index)                                                  \
  (typeof((list).data))(_list_get((list).data, (list).count,                   \
                                  (list).element_size, (index)))

#define list_head(list)                                                        \
  (typeof((list).data))(_list_head((list).data, &(list).count,                 \
                                   (list).element_size))

#define list_tail(list)                                                        \
  (typeof((list).data))(_list_tail((list).data, &(list).count,                 \
                                   (list).element_size))

#endif
