#ifndef my_util_h
#define my_util_h

typedef enum { LESS, EQUAL, GREATER, LESS_EQ, GREATER_EQ } comparison;

#define RETURN_COMPARE(a, b)                                                   \
  do {                                                                         \
    if (a > b)                                                                 \
      return GREATER;                                                          \
    else if (a >= b)                                                           \
      return GREATER_EQ;                                                       \
    else if (a == b)                                                           \
      return EQUAL;                                                            \
    else if (a <= b)                                                           \
      return LESS_EQ;                                                          \
                                                                               \
    return LESS;                                                               \
  } while (0)

#define bubblesort(list, count, compare_function, el_type)                     \
  do {                                                                         \
    bool changed = 0;                                                          \
    size_t n = (count);                                                        \
    if (n != 0) {                                                              \
      do {                                                                     \
        changed = 0;                                                           \
        for (size_t i = 0; i < n - 1; i++) {                                   \
          if ((compare_function)((list)[i], (list)[i + 1]) == GREATER) {       \
            el_type temp_a = (list)[i];                                        \
            (list)[i] = (list)[i + 1];                                         \
            (list)[i + 1] = temp_a;                                            \
            changed = 1;                                                       \
          }                                                                    \
        }                                                                      \
        n--;                                                                   \
      } while (changed);                                                       \
    }                                                                          \
  } while (0)

#endif
