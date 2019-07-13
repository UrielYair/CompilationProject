#include "SymbolTable.h"
#include "ht_hash_table.h"
#include "Token.h"
#include "slist.h"
#include "ID_Information.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// TODO: remember to free memory in the end.
// TODO: think about a way to transfer id's list into the block to 
//		 eliminate ability of declere a variable that is in the parameters list.

SymbolTable* currentTable = NULL;

SymbolTable* make_table() {
		
	SymbolTable* newTable = (SymbolTable*)malloc(sizeof(SymbolTable));
	
	if (newTable != NULL)
	{
		newTable->father = currentTable;
		newTable->currentSymbolTable = ht_new();
		currentTable = newTable;
	}
	
	return currentTable;
}

SymbolTable* pop_table() {
	ht_del_hash_table(currentTable->currentSymbolTable);
	currentTable = currentTable->father;
	return currentTable;
}

ID_Information* insert(char* id_name) {
	ID_Information* info = lookup(id_name);
	if (info != NULL)
	{
		printf("The id (%s) is already defined. - line: %u.\n", id_name, getCurrentToken()->lineNumber);
		return NULL;
	}
	info = new_ID_Information(id_name);
	ht_insert(currentTable->currentSymbolTable, id_name, info);
	return info;
}

ID_Information* lookup(char* id_name) {
	return ht_search(currentTable->currentSymbolTable, id_name);
}

ID_Information* find(char* id_name) {
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

void set_id_info_pointer(ID_Information * id_entry, char* whatToUpdate, void* value) {

	if (id_entry == NULL)
	{
		printf("No id_entry to set.\n");
		return;
	}

	// name
	if (strcmp(whatToUpdate, "name") == 0)
		strcpy(id_entry->name, (char*)value);

	// functionOrVariable
	else if (strcmp(whatToUpdate, "functionOrVariable") == 0)
		strcpy(id_entry->functionOrVariable, (char*)value);

	// ID_Type
	else if (strcmp(whatToUpdate, "ID_Type") == 0)
		strcpy(id_entry->ID_Type, (char*)value);

	// returnedType
	else if (strcmp(whatToUpdate, "returnedType") == 0)
		strcpy(id_entry->returnedType, (char*)value);

	// listOfArguments
	else if (strcmp(whatToUpdate, "listOfArguments") == 0)
		id_entry->listOfArguments = (struct slist*)value;

	else
		printf("member not found for the set request.\n");

}
void set_id_info_boolean(ID_Information* id_entry, char* whatToUpdate, bool value) {

	if (id_entry == NULL)
	{
		printf("No id_entry to set.\n");
		return;
	}

	// wasUsed
	if (strcmp(whatToUpdate, "wasUsed") == 0)
		id_entry->wasUsed = value;

	// isArray
	else if (strcmp(whatToUpdate, "isArray") == 0)
		id_entry->isArray = value;

	else
		printf("member not found for the set request.\n");
}
void set_id_info_integer(ID_Information* id_entry, char* whatToUpdate, int value) {

	if (id_entry == NULL)
	{
		printf("No id_entry to set.\n");
		return;
	}

	// sizeOfArray
	if (strcmp(whatToUpdate, "sizeOfArray") == 0)
		id_entry->sizeOfArray = value;

	// numOfArguments
	else if (strcmp(whatToUpdate, "numOfArguments") == 0)
	id_entry->numOfArguments = value;

	else
		printf("member not found for the set request.\n");
}

char* get_id_type(ID_Information * id_entry) {
	if (strcmp(id_entry->functionOrVariable, "variable") == 0)
		return id_entry->ID_Type;
	else
		return id_entry->returnedType;
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
	else
		set_id_info_boolean(find(id_name), "wasUsed", true);
}


