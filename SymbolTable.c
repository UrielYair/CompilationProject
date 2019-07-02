#include "SymbolTable.h"
#include <string.h>
#include <stdlib.h>

// TODO: check if allocation successeded. (malloc/calloc/realloc)
// TODO: add free function wherever allocation be done.	

SymbolTable* currentTable = NULL;

SymbolTable* make_table() {
		
	SymbolTable* newTable;
	newTable->father = currentTable;
	newTable->currentSymbolTable = ht_new();

	currentTable = newTable;
	return currentTable;
}

SymbolTable* pop_table() {
	ht_del_hash_table(currentTable->currentSymbolTable);
	currentTable = currentTable->father;
	return currentTable;
}


ID_Information* insert(char* id_name) {
	// TODO: check if already in the table.
	ID_Information* info = lookup(id_name);
	if (info != NULL) // TODO: add error printing.
		return NULL;	
	info = (ID_Information*)malloc(sizeof(ID_Information));
	info->name = strdup(id_name);
	ht_insert(currentTable->currentSymbolTable, id_name, info);
	return info;
}

ID_Information* lookup(char* id_name) {
	// TODO: maybe error printing is needed.
	return ht_search(currentTable->currentSymbolTable, id_name);
}

ID_Information* find(char* id_name) {
	// TODO: maybe error printing is needed.
	SymbolTable* tempSymbolTableToCheck = currentTable;
	while (tempSymbolTableToCheck != NULL)
	{
		ID_Information* result = ht_search(currentTable->currentSymbolTable, id_name);
		if (result != NULL)
			return result;
		tempSymbolTableToCheck = tempSymbolTableToCheck->father;
	}
	return NULL;

}


// TODO: keep editing !!!
void set_id_type(ID_Information* id_entry, char* id_type) {
	id_entry->name = strdup(id_type);
}
char* get_id_type(ID_Information* id_entry) {
	if(strcmp(id_entry->functionOrVariable,"variable")==0)
		return id_entry->ID_Type;
	else
		return id_entry->returnedType;
}
void free_ID_info(ID_Information* id_entry) {
	// TODO: Implement!!
}


/*
functions from hash table for reference.

ht_hash_table*	ht_new();
void			ht_del_hash_table(ht_hash_table* ht);

void			ht_insert(ht_hash_table* ht, const char* key, const ID_Information* value);
ID_Information* ht_search(ht_hash_table* ht, const char* key);
void			ht_delete(ht_hash_table* h, const char* key);
*/
