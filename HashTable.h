#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "SymbolTable.h"

// ht_item is an item in the hash table
typedef struct ht_item {
	char* key;
	ID_Information* value;
} ht_item;

typedef struct ht_hash_table {
	int size_index;
	int size;
	int count;
	ID_Information** items;
} ht_hash_table;

ht_hash_table* ht_new();
void ht_del_hash_table(ht_hash_table* ht);

void ht_insert(ht_hash_table* ht, const char* key, const ID_Information* value);
ID_Information* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);


#endif  // HASH_TABLE_H



