#include <stdlib.h>
#include "ht_item.h"
#include "xmalloc.h"
#include <string.h>

// HT_DELETED_ITEM is used to mark a bucket containing a deleted item
ht_item HT_DELETED_ITEM = { NULL, NULL };


/*
 * Initialises a new item containing k: v
 */
ht_item* ht_new_item(char* k, ID_Information* v) {
	ht_item* i = xmalloc(sizeof(ht_item));
	strcpy(i->key,k);
	i->value = v;
	return i;
}


/*
 * Deletes the ht_item i
 */
void ht_del_item(ht_item* i) {
	free(i->key);
	delete_ID_Information(i->value); // free(i->value); 
	free(i);
}


