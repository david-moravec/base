#include "../include/arena.h"
#include "../include/list.h"
#include "../utest/utest.h"

TEST(test_init) {
  Arena *arena = arena_alloc(4096, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  ASSERT((list.element_size == sizeof(int)), "");
  ASSERT((list.size == 0), "");
  ASSERT((list.count == 0), "");
  ASSERT((list.arena == arena), "");
}

TEST(test_get_bounds) {
  Arena *arena = arena_alloc(4096, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  for (int i = 0; i < 10; i++) {
    int v = i * i;
    list_push(list, v);
  }
  for (int i = 0; i < 10; i++) {
    ASSERT((list_get(list, i) == i * i), "");
  }
  ASSERT((list.count == 10), "");
}

TEST(test_head) {
  Arena *arena = arena_alloc(4096, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  for (int i = 0; i < 5; i++) {
    int v = i + 100;
    list_push(list, v);
  }
  int *head = list_head(list);
  ASSERT((*head == 100), "");

  /* writing through the head pointer should be visible via list_get */
  *head = 999;
  ASSERT((list_get(list, 0) == 999), "");
}

TEST(test_tail) {
  Arena *arena = arena_alloc(4096, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  for (int i = 0; i < 5; i++) {
    int v = i + 100;
    list_push(list, v);
  }
  int *tail = list_tail(list);
  ASSERT((*tail == 104), "");

  *tail = 999;
  ASSERT((list_get(list, list.count - 1) == 999), "");
}

TEST(test_pop) {
  Arena *arena = arena_alloc(4096, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  for (int i = 0; i < 5; i++) {
    int v = i;
    list_push(list, v);
  }
  ASSERT((list.count == 5), "");

  /* pop should return values LIFO: 4, 3, 2, 1, 0 */
  for (int expected = 4; expected >= 0; expected--) {
    int popped = list_pop(list);
    ASSERT((popped == expected), "");
  }
  ASSERT((list.count == 0), "");
}

TEST(test_remove_middle) {
  Arena *arena = arena_alloc(4096, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  for (int i = 0; i < 10; i++) {
    int v = i;
    list_push(list, v);
  }
  /* remove index 5 (value 5); remaining values shift left */
  int removed = 0;
  list_remove(list, 5, &removed);
  ASSERT((removed == 5), "");
  ASSERT((list.count == 9), "");

  int expected[] = {0, 1, 2, 3, 4, 6, 7, 8, 9};
  for (int i = 0; i < 9; i++) {
    ASSERT((list_get(list, i) == expected[i]), "");
  }
}

TEST(test_remove_first) {
  Arena *arena = arena_alloc(4096, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  for (int i = 0; i < 5; i++) {
    int v = i;
    list_push(list, v);
  }
  int removed = -1;
  list_remove(list, 0, &removed);
  ASSERT((removed == 0), "");
  ASSERT((list.count == 4), "");
  ASSERT((list_get(list, 0) == 1), "");
}

TEST(test_remove_last) {
  Arena *arena = arena_alloc(4096, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  for (int i = 0; i < 5; i++) {
    int v = i;
    list_push(list, v);
  }
  int removed = 0;
  list_remove(list, list.count - 1, &removed);
  ASSERT((removed == 4), "");
  ASSERT((list.count == 4), "");
}

TEST(test_append) {
  Arena *arena = arena_alloc(4096, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);

  /* list_append grows the list by one and returns a pointer to the
   * new (uninitialized/default) slot, which we then fill in */
  for (int i = 0; i < 20; i++) {
    int *slot = list_append(list);
    *slot = i * 2;
  }
  ASSERT((list.count == 20), "");
  for (int i = 0; i < 20; i++) {
    ASSERT((list_get(list, i) == i * 2), "");
  }
}

TEST(test_iterate) {
  Arena *arena = arena_alloc(1 << 20, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  int sum_expected = 0;
  for (int i = 0; i < 50; i++) {
    int v = i;
    list_push(list, v);
    sum_expected += i;
  }

  int sum_actual = 0;
  size_t idx;
  for (list_iterate(list, idx)) {
    int current_elemtn = list_get(list, idx);
    sum_actual += current_elemtn;
  }
  ASSERT((sum_actual == sum_expected), "");
}

TEST(test_growth) {
  /* size should grow to accommodate count as elements are pushed,
   * and should always be >= count */
  Arena *arena = arena_alloc(1 << 20, 0);
  typedef List(int) ListInt;
  ListInt list;
  list_init(&list, arena);
  for (int i = 0; i < 1000; i++) {
    int v = i;
    list_push(list, v);
    ASSERT((list.size >= list.count), "");
  }
  ASSERT((list.count == 1000), "");
  for (int i = 0; i < 1000; i++) {
    ASSERT((list_get(list, i) == i), "");
  }
}

TEST(test_struct_head_tail_pop_remove) {
  Arena *arena = arena_alloc(4096, 0);
  typedef struct {
    double a;
    double b;
  } Foo;
  typedef List(Foo) ListFoo;
  ListFoo list;
  list_init(&list, arena);

  for (int i = 0; i < 10; i++) {
    Foo f = {.a = i, .b = i * 10.0};
    list_push(list, f);
  }

  Foo *head = list_head(list);
  ASSERT((head->a == 0 && head->b == 0.0), "");

  Foo *tail = list_tail(list);
  ASSERT((tail->a == 9 && tail->b == 90.0), "");

  Foo popped = list_pop(list);
  ASSERT((popped.a == 9 && popped.b == 90.0), "");
  ASSERT((list.count == 9), "");

  Foo removed;
  list_remove(list, 0, &removed);
  ASSERT((removed.a == 0 && removed.b == 0.0), "");
  ASSERT((list.count == 8), "");
  Foo new_head = list_get(list, 0);
  ASSERT((new_head.a == 1 && new_head.b == 10.0), "");
}

TEST(test_independent_lists_same_arena) {
  Arena *arena = arena_alloc(8192, 0);
  typedef List(int) ListInt;
  typedef List(double) ListDouble;

  ListInt li;
  ListDouble ld;
  list_init(&li, arena);
  list_init(&ld, arena);

  for (int i = 0; i < 20; i++) {
    int v = i;
    list_push(li, v);
  }
  for (int i = 0; i < 20; i++) {
    double v = i * 0.5;
    list_push(ld, v);
  }

  ASSERT((li.count == 20), "");
  ASSERT((ld.count == 20), "");
  for (int i = 0; i < 20; i++) {
    ASSERT((list_get(li, i) == i), "");
    ASSERT((list_get(ld, i) == i * 0.5), "");
  }
}

int main() {
  RUN_TEST(test_init);
  RUN_TEST(test_get_bounds);
  RUN_TEST(test_head);
  RUN_TEST(test_tail);
  RUN_TEST(test_pop);
  RUN_TEST(test_remove_middle);
  RUN_TEST(test_remove_first);
  RUN_TEST(test_remove_last);
  RUN_TEST(test_append);
  RUN_TEST(test_iterate);
  RUN_TEST(test_growth);
  RUN_TEST(test_struct_head_tail_pop_remove);
  RUN_TEST(test_independent_lists_same_arena);
  SUMMARY();
}
