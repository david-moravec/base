#include <string.h>

#include "dict.h"
#include "list.h"

LIST_FUNCS_DEFINE(Entry, struct Entry);

static uint32_t hashString(const char *key, int length) {
  uint32_t hash = 2166136261u;
  for (int i = 0; i < length; i++) {
    hash ^= (uint8_t)key[i];
    hash *= 16777619;
  }
  return hash;
}

static struct Entry *find_entry(struct list_Entry *entries, size_t capacity,
                                uint32_t hash) {
  if (capacity == 0) {
    return NULL;
  }

  int index = hash % capacity;

  for (;;) {
    struct Entry *entry = list_Entry_get(entries, index);

    if (entry->hash == 0) {
      if (entry->value == NULL) {
        return entry;
      }
    } else if (entry->hash == hash) {
      return entry;
    }

    // wrap to beginning
    index = (index + 1) % capacity;
  }

  return NULL;
}

static void grow_dict(Dict *dict) {
  int old_capacity = dict->container.list.size;
  int new_cap = 2 * old_capacity;
  struct list_Entry entries;
  list_Entry_init(&entries, new_cap);

  for (int i = 0; i < new_cap; i++) {
    struct Entry *entry = &entries.list.data[i];
    entry->hash = -1;
    entry->key = 0;
    entry->value = NULL;
  }

  // copy existing
  for (int i = 0; i < dict->container.list.size; i++) {
    struct Entry *entry = &dict->container.list.data[i];

    if (entry->hash == -1)
      continue;

    struct Entry *dest = find_entry(&entries, new_cap, entry->hash);
    dest->key = entry->key;
    dest->value = entry->value;
  }

  list_Entry_free(&dict->container);
  dict->container.list = entries.list;
  dict->capacity = new_cap;
}

bool dict_get(Dict *dict, const char *key, void *value) {
  struct Entry *entry = find_entry(&dict->container, dict->container.list.size,
                                   hashString(key, strlen(key)));

  if (entry == NULL || entry->key == 0 && entry->hash == 0) {
    return false;
  }

  // assign pointer address
  memcpy(value, &entry->value, sizeof(void *));
  return true;
}

bool dict_get_copy(Dict *dict, const char *key, void *value, size_t val_size) {
  void **val = NULL;

  if (dict_get(dict, key, val)) {
    memcpy(value, val, val_size);
    return true;
  }

  return false;
}

bool dict_set(Dict *dict, const char *key, void *value) {
  if (dict->count + 1 > MAX_LOAD_FACTOR * dict->capacity) {
    grow_dict(dict);
  }
  uint32_t hash = hashString(key, strlen(key));
  struct Entry *entry =
      find_entry(&dict->container, dict->container.list.size, hash);

  bool newEntry = entry->key == 0;

  if (newEntry && entry->value == NULL) {
    dict->count++;
  }

  entry->key = key;
  entry->hash = hash;
  entry->value = value;

  return newEntry;
}

bool dict_set_copy(Dict *dict, const char *key, void *value, size_t val_size) {
  void *copy = malloc(val_size);
  memcpy(copy, value, val_size);
  return dict_set(dict, key, value);
}

void dict_init(Dict *dict) {
  list_Entry_init(&dict->container, 8);
  for (size_t i = 0; i < dict->container.list.size; i++) {
    struct Entry *entry = &dict->container.list.data[i];
    entry->hash = 0;
    entry->key = 0;
    entry->value = NULL;
    dict->container.list.count++;
  };

  dict->capacity = 8;
  dict->count = 0;
}

struct Entry **dict_get_items(Dict *dict, size_t *count) {
  struct Entry **items = (struct Entry **)malloc(dict->count);

  size_t j = 0;

  for (int i = 0; i < dict->capacity; i++) {
    struct Entry *item = list_Entry_get(&dict->container, i);

    if (item->value != NULL) {
      items[j] = item;
      j++;
    }
  }
  *count = j;

  return items;
}

void dict_free(Dict *dict) { list_Entry_free(&dict->container); }
