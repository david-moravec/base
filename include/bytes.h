#include "arena.h"
#include "list.h"
#include <stdint.h>

typedef List(uint8_t) ByteList;

struct Bytes {
  ByteList list;
};
