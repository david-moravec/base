#ifndef MY_CUSTOM_LIST_H
#define MY_CUSTOM_LIST_H

#include <stdlib.h>

struct list {
  size_t size;
  size_t count;
  size_t el_size;
  void *data;
};

void list_init(struct list *list, size_t el_size, size_t init_size);
void list_free(struct list *list);

void *list_append(struct list *list, size_t *index);
void *list_append_copy(struct list *list, void *element, size_t *index);
void *list_append_many(struct list *list, void *elements, size_t len_elements,
                       size_t *end_index);
void *list_insert_copy(struct list *list, void *element, size_t index);
void *list_remove(struct list *list, size_t index);
void *list_get(struct list *list, size_t index);
void *list_head(struct list *list);
void *list_tail(struct list *list);

void list_grow(struct list *list);

#define LIST_TYPE_DEFINE(name, type)                                           \
  struct list_##name {                                                         \
    struct list list;                                                          \
  };

#define LIST_FUNCS_DEFINE(name, type)                                          \
  static inline void list_##name##_init(struct list_##name *list,              \
                                        size_t init_size) {                    \
    return list_init(&list->list, sizeof(type), init_size);                    \
  }                                                                            \
  static inline void list_##name##_free(struct list_##name *list) {            \
    return list_free(&list->list);                                             \
  }                                                                            \
  static inline type *list_##name##_append(struct list_##name *list,           \
                                           size_t *index) {                    \
    return list_append(&list->list, index);                                    \
  }                                                                            \
  static inline type *list_##name##_append_copy(                               \
      struct list_##name *list, type *element, size_t *index) {                \
    return list_append_copy(&list->list, element, index);                      \
  }                                                                            \
  static inline type *list_##name##_append_many(                               \
      struct list_##name *list, type *elements, size_t len_elements,           \
      size_t *end_index) {                                                     \
    return list_append_many(&list->list, elements, len_elements, end_index);   \
  }                                                                            \
  static inline type *list_##name##_insert_copy(struct list_##name *list,      \
                                                type *element, size_t index) { \
    return list_insert_copy(&list->list, element, index);                      \
  }                                                                            \
  static inline type *list_##name##_remove(struct list_##name *list,           \
                                           size_t index) {                     \
    return list_remove(&list->list, index);                                    \
  }                                                                            \
  static inline type *list_##name##_get(struct list_##name *list,              \
                                        size_t index) {                        \
    return list_get(&list->list, index);                                       \
  }                                                                            \
  static inline type *list_##name##_head(struct list_##name *list) {           \
    return list_head(&list->list);                                             \
  }                                                                            \
  static inline type *list_##name##_tail(struct list_##name *list) {           \
    return list_tail(&list->list);                                             \
  }

#define LIST_DEFINE(name, type)                                                \
  LIST_TYPE_DEFINE(name, type)                                                 \
  LIST_FUNCS_DEFINE(name, type)

#endif
