#ifndef HT_HASH_TABLE_H
#define HT_HASH_TABLE_H

#include "ID_Information.h"
#include "ht_item.h"

typedef struct ht_hash_table {
	int size_index;
	int size;
	int count;
	ht_item** items;
} ht_hash_table;

ht_hash_table* ht_new();
void ht_del_hash_table(ht_hash_table* ht);

void ht_insert(ht_hash_table* ht, char* key, ID_Information* value);
ID_Information* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);


static ht_hash_table* ht_new_sized(const int size_index);
static void ht_resize(ht_hash_table* ht, const int direction);
static int ht_generic_hash(const char* s, const int a, const int m);
static int ht_hash(const char* s, const int num_buckets, const int attempt);


#endif  // HT_HASH_TABLE_H



