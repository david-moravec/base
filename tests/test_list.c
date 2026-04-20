
#include "collections/list.h"
#include "utest.h"

TEST(test_push) {
  typedef List(float) ListFloat;
  ListFloat list_f;
  for (int i = 0; i < 100; i++) {
    float a = i * 1.0;
    list_push(list_f, &a);
  }

  ASSERT((list_f.count) == 100, "");
  ASSERT((list_f.element_size) == sizeof(float), "");

  for (int i = 0; i < 100; i++) {
    ASSERT((*list_get(list_f, i) == i * 1.0), "");
  }
}

int main() {
  typedef List(int) ListInt;
  ListInt foo;
  list_append(foo);
  int a = 5;
  float f = 5.0;
  list_push(foo, &a);
}
