#include "SymbolTable.h"
#include "HashTable.c"
#include "Token.h"

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


ID_Information* new_ID_Information(char* name) {

	ID_Information* new_id = (ID_Information*)malloc(sizeof(ID_Information));

	if (new_id != NULL)
	{
		new_id->name = strdup(name);
		new_id->functionOrVariable = NULL;
		new_id->wasUsed = false;
		new_id->ID_Type = NULL;
		new_id->sizeOfArray = -1;
		new_id->isArray = false;
		new_id->returnedType = NULL;
		new_id->numOfArguments = -1;
		new_id->listOfArguments = NULL;
	}
	return new_id;
}

void delete_ID_Information(ID_Information* idToDelete) {

	free(idToDelete->name);

	if (strcmp(idToDelete->functionOrVariable, "function") == 0)
	{
		free(idToDelete->returnedType);
		// TODO: call delete list.
		//idToDelete->listOfArguments
		free(idToDelete->listOfArguments);
	}
	free(idToDelete->functionOrVariable);
	free(idToDelete->ID_Type);
	
	free(idToDelete);
}

ID_Information* insert(char* id_name) {
	ID_Information* info = lookup(id_name);
	if (info != NULL) // TODO: add error printing.
		return NULL;
	info = new_ID_Information(id_name);
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

void set_id_info(ID_Information* id_entry, char* whatToUpdate, void* value) {
	
	// name
	if (strcmp(whatToUpdate, "name") == 0)
		id_entry->name = strdup((char*)value);
	
	// functionOrVariable
	else if (strcmp(whatToUpdate, "functionOrVariable") == 0)
		id_entry->functionOrVariable = strdup((char*)value);
	
	// wasUsed
	else if (strcmp(whatToUpdate, "wasUsed") == 0)
		id_entry->wasUsed = (bool)value;
	
	// ID_Type
	else if (strcmp(whatToUpdate, "ID_Type") == 0)
		id_entry->ID_Type = strdup((char*)value);
	
	// sizeOfArray
	else if (strcmp(whatToUpdate, "sizeOfArray") == 0)
		id_entry->sizeOfArray = (int)value;
	
	// returnedType
	else if (strcmp(whatToUpdate, "returnedType") == 0)
		id_entry->returnedType = strdup((char*)value);
	
	// numOfArguments
	else if (strcmp(whatToUpdate, "numOfArguments") == 0)
		id_entry->numOfArguments = (int)value;
	
	// listOfArguments
	else if (strcmp(whatToUpdate, "listOfArguments") == 0)
		id_entry->listOfArguments = (struct IDInfoLinkNode*)value;
	
	// isArray
	else if (strcmp(whatToUpdate, "isArray") == 0)
		id_entry->listOfArguments = (bool)value;
}
char* get_id_type(ID_Information* id_entry) {
	if(strcmp(id_entry->functionOrVariable,"variable")==0)
		return id_entry->ID_Type;
	else
		return id_entry->returnedType;
}


bool wereAllIDsUsed () 
{ 
	bool everyIDWasInUse = true;
	int counterOfIDsWhichNotUsed = 0;

	// Iterate through existing hash table.
	for (int i = 0; i < currentTable->currentSymbolTable->size; i++) 
	{
		ht_item* item = currentTable->currentSymbolTable->items[i];
		if (item != NULL && item != &HT_DELETED_ITEM) 
		{
			ID_Information* currentID = item->value;
			if (currentID->wasUsed == false)
			{
				everyIDWasInUse = false;
				printf("The ID: %s wasn't in use in the current scope.\n", currentID->name);
				counterOfIDsWhichNotUsed++;
			}
		}
	}

	if (everyIDWasInUse)
		printf("All the ID's were used.\n");
	else
		printf("%u ID's wasn't in use.\n", counterOfIDsWhichNotUsed);

	return everyIDWasInUse;
}

bool isIDExistInSymbolTable(char* id_name) {
	ID_Information* id = find(id_name);
	if (id == NULL)
		return false;
	return true;
}

void checkIfIDAlreadyDeclared(char* id_name) {
	if (!isIDExistInSymbolTable(id_name))
		printf("The ID: %s in line %u must be declared before being used.\n", id_name, getCurrentToken()->lineNumber);
}

bool isFunction(char* id_name) {
	ID_Information* idToCheck = find(id_name);
	if (strcmp(idToCheck->functionOrVariable, "function") == 0)
		return true;
	return false;
}