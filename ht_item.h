#ifndef HT_ITEM_H
#define HT_ITEM_H

#include "ID_Information.h"

// ht_item is an item in the hash table
typedef struct ht_item {
	char* key;
	ID_Information* value;
} ht_item;

static ht_item* ht_new_item(char* k, ID_Information* v);
static void ht_del_item(ht_item* i);


#endif // HT_ITEM_H