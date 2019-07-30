#include "SymbolTable.h"
#include "ht_hash_table.h"
#include "Token.h"
#include "slist.h"
#include "ID_Information.h"
#include "Utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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
		fprintf(semanticOutput, "id (%s) is already defined. - line: %u.\n", id_name, getCurrentToken()->lineNumber);
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
		ID_Information* result = ht_search(tempSymbolTableToCheck->currentSymbolTable, id_name);
		if (result != NULL)
			return result;
		tempSymbolTableToCheck = tempSymbolTableToCheck->father;
	}
	return NULL;

}

void set_id_info_pointer(ID_Information * id_entry, char* whatToUpdate, void* value) {

	if (id_entry == NULL)
	{
		return;
	}

	// name
	if (strcmp(whatToUpdate, "name") == 0)
		id_entry->name = _strdup((char*)value);

	// functionOrVariable
	else if (strcmp(whatToUpdate, "functionOrVariable") == 0)
		id_entry->functionOrVariable = _strdup((char*)value);

	// ID_Type
	else if (strcmp(whatToUpdate, "ID_Type") == 0)
		id_entry->ID_Type = _strdup((char*)value);

	// returnedType
	else if (strcmp(whatToUpdate, "returnedType") == 0)
		id_entry->returnedType = _strdup((char*)value);

	// listOfArguments
	else if (strcmp(whatToUpdate, "listOfArguments") == 0)
		id_entry->listOfArguments = (struct slist*)value;

	else
		fprintf(semanticOutput, "member not found for the set request.\n");

}
void set_id_info_boolean(ID_Information* id_entry, char* whatToUpdate, bool value) {

	if (id_entry == NULL)
	{
		return;
	}

	// wasUsed
	if (strcmp(whatToUpdate, "wasUsed") == 0)
		id_entry->wasUsed = value;

	// isArray
	else if (strcmp(whatToUpdate, "isArray") == 0)
		id_entry->isArray = value;

	else
		fprintf(semanticOutput, "member not found for the set request.\n");
}
void set_id_info_integer(ID_Information* id_entry, char* whatToUpdate, int value) {

	if (id_entry == NULL)
	{
		return;
	}

	// sizeOfArray
	if (strcmp(whatToUpdate, "sizeOfArray") == 0)
		id_entry->sizeOfArray = value;

	// numOfArguments
	else if (strcmp(whatToUpdate, "numOfArguments") == 0)
	id_entry->numOfArguments = value;

	else
		fprintf(semanticOutput, "member not found for the set request.\n");
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
	ID_Information* id = NULL;

	if (!isIDExistInSymbolTable(id_name))
		fprintf(semanticOutput, "id (%s) must be declared before being used. - line %u.\n", id_name, getCurrentToken()->lineNumber);
	else
	{
		id = find(id_name);
		if (id!=NULL)
			set_id_info_boolean(id, "wasUsed", true);
	}
}


