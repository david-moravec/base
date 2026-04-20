#include "arena.h"
#include "collections/list.h"
#include "utest.h"

TEST(test_push) {
  Arena *arena = arena_alloc(4096, 0);

  typedef List(float) ListFloat;
  ListFloat list_f;
  list_init(&list_f, arena);

  ASSERT((list_f.element_size == sizeof(float)), "");
  ASSERT((list_f.size == 0), "");
  ASSERT((list_f.count == 0), "");

  for (int i = 0; i < 100; i++) {
    float a = i * 1.0;
    list_push(list_f, &a);
  }

  ASSERT((list_f.count) == 100, "");

  for (int i = 0; i < 100; i++) {
    ASSERT((*list_get(list_f, i) == i * 1.0), "");
  }
}

TEST(test_push_foo) {
  Arena *arena = arena_alloc(4096, 0);

  typedef struct {
    double a;
    double b;
  } Foo;

  typedef List(Foo) ListFoo;
  ListFoo list_f;
  list_init(&list_f, arena);

  ASSERT((list_f.element_size == sizeof(Foo)), "");
  ASSERT((list_f.size == 0), "");
  ASSERT((list_f.count == 0), "");

  for (int i = 0; i < 100; i++) {
    Foo a;
    a.a = (i + 1);
    a.b = (i + 1) * 2.0;
    list_push(list_f, &a);
  }

  ASSERT((list_f.count) == 100, "");

  for (int i = 0; i < 100; i++) {
    Foo *a = list_get(list_f, i);
    ASSERT((a->a == i + 1), "");
    ASSERT((a->b == (i + 1) * 2.0), "");
  }
}

int main() {
  RUN_TEST(test_push);
  RUN_TEST(test_push_foo);
  SUMMARY();
}
