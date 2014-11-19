#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct something {
  int field;
} something;

typedef struct hash_table_entry {
  char* key;
  void* value;
} hash_table_entry;

typedef struct hash_table {
  int count;
  int size;
  hash_table_entry** entries;
} hash_table;

unsigned long hash(char* str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}

hash_table_entry* hash_table_entry_create(char* key, void* smth) {
  hash_table_entry* entry = malloc(sizeof(hash_table_entry));
  entry->key = malloc(strlen(key) + 1);
  strcpy(entry->key, key);
  entry->value = smth;
  return entry;
}

void hash_table_entry_free(hash_table_entry* entry) {
  free(entry->key);
  free(entry);
}

hash_table* hash_table_create(void) {
  hash_table* table = malloc(sizeof(hash_table));
  table->count = 0;
  table->size = 100;
  table->entries = NULL;
  table->entries = malloc(sizeof(hash_table_entry) * table->size);
  return table;
}

void hash_table_free(hash_table* table) {
  hash_table_entry* entry;
  for (int i = 0; i < table->size; i++) {
    entry = table->entries[i];
    if (entry) {
      hash_table_entry_free(entry);
    }
  }
  free(table);
}

void hash_table_add(hash_table* table, char* key, void* smth) {
  unsigned long hashval = hash(key);
  unsigned int index = hashval % table->size;
  hash_table_entry* entry = hash_table_entry_create(key, smth);
  table->entries[index] = entry;
}

void* hash_table_get(hash_table* table, char* key) {
  unsigned long hashval = hash(key);
  unsigned int index = hashval % table->size;
  hash_table_entry* entry;
  entry = table->entries[index];
  if (entry != NULL) {
    return entry->value;
  } else {
    return NULL;
  }
}

int main(int argc, const char *argv[]) {
  hash_table* table = hash_table_create();

  something smth;
  smth.field = 2;
  hash_table_add(table, "something", &smth);
  something* smth2 = hash_table_get(table, "something");
  printf("Asked for 'something'->field, got: %d\n", smth2->field);

  char* data = "dsadsdsfdsfasfds";
  hash_table_add(table, "wat", data);
  char* retrieved = hash_table_get(table, "wat");
  printf("Asked for 'wat', got: %s\n", retrieved);

  hash_table_free(table);

  return 0;
}

