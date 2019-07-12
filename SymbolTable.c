#include "SymbolTable.h"
#include "HashTable.c"
#include "Token.h"
#include "slist.h"
#include <string.h>
#include <stdlib.h>

// TODO: remember to free memory in the end.
// TODO: think about a way to transfer id's list into the block to 
//		 eliminate ability of declere a variable that is in the parameters list.

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
		slist_delete(idToDelete->listOfArguments);
		free(idToDelete->listOfArguments);
	}
	free(idToDelete->functionOrVariable);
	free(idToDelete->ID_Type);
	
	free(idToDelete);
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
			
			// checking if one of the variable was not in use.
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
	else
		set_id_info(find(id_name),"wasUsed",true);
}

bool isFunction(char* id_name) {
	ID_Information* idToCheck = find(id_name);
	if (strcmp(idToCheck->functionOrVariable, "function") == 0)
		return true;
	return false;
}

void checkFunctionArguments(char* id_name, slist* argumentsOfFunction) {
	
	// Check if the id is a function.
	if (!isFunction(id_name))
	{
		prinf("Can't check the arguments because %s is not a function!\n", id_name);
		return;
	}

	ID_Information* idToCheck = find(id_name);
	slist* argumentdsOfIdToCheck = idToCheck->listOfArguments;
	snode* a, * b;
	
	// Checking if the amount of argument is equal to the 
	// recieved variables in the functions call.
	int listASize, listBSize;
	listASize = slist_get_count(argumentdsOfIdToCheck);
	listBSize = slist_get_count(argumentsOfFunction);
	if (listASize != listBSize)
	{
		printf("The amount of parameters in the function call doesn't match the function definition.\n");
		return;
	}

	for (a = argumentdsOfIdToCheck->head->data, b = argumentsOfFunction->head->data	;
		a->next != NULL && b->next !=NULL	;
		a = a->next, b = b->next)
	{
		ID_Information* A = (ID_Information*)a;
		ID_Information* B = (ID_Information*)b;
		if (!isAValueCanHoldBValue(A,B))
			printf("Types mismatch. %s can't hold %s.\n");
	}
}

bool isAValueCanHoldBValue(ID_Information* A, ID_Information* B) {
	
	char* aType = strdup(find(A->name)->ID_Type);
	char* bType = strdup(find(B->name)->ID_Type);

	if (isFunction(A->name))
	{
		printf("ID of function type can not hold variable value.\n");
		return false;
	}

	if (isFunction(B->name))
	{
		if ((strcmp(aType, "integer") == 0 && strcmp(B->returnedType, "integer") == 0)
			||
			(strcmp(aType, "real") == 0 && strcmp(B->returnedType, "integer") == 0)
			||
			(strcmp(aType, "real") == 0 && strcmp(B->returnedType, "real") == 0))
			return true;
	}
	

	if ((strcmp(aType, "integer") == 0 && strcmp(bType, "integer") == 0)
		||
		(strcmp(aType, "real") == 0 && strcmp(bType, "integer") == 0)
		||
		(strcmp(aType, "real") == 0 && strcmp(bType, "real") == 0))
		return true;
	
	prinf("%s can not get the value of %s as a variable.\n", A->name, B->name);
	return false;
}
bool checkBoundaries(int indexInArray, int sizeOfArray) {
	if (indexInArray >= 0 && indexInArray <= sizeOfArray)
		return true;
	printf("Invalid index for the array.\n");
	return false;
}
bool assighnmentTypeChecking(char* leftType, char* rightType, int lineNumberWithAssighnment) {

	if (strcmp(leftType, "error_type") == 0 || strcmp(rightType, "error_type") == 0)
	{
		prinf("%s can not be saved into %s. line number: %u.\n", rightType, leftType, lineNumberWithAssighnment);
		return false;
	}

	if ((strcmp(leftType, "integer") == 0 && strcmp(rightType, "integer") == 0)
		||
		(strcmp(leftType, "real") == 0 && strcmp(rightType, "integer") == 0)
		||
		(strcmp(leftType, "real") == 0 && strcmp(rightType, "real") == 0))
		return true;

	prinf("%s can not be saved into %s. line number: %u.\n", rightType, leftType, lineNumberWithAssighnment);
	return false;

}
char* arithmeticTypeChecking(char* operandA, char* operandB) {
	if (strcmp(operandA, operandB) == 0)
		return operandA;
	if (strcmp(operandA, "error_type") == 0 || strcmp(operandB, "error_type") == 0)
		return "error_type";
	if (strcmp(operandA, "real") == 0 || strcmp(operandB, "real") == 0)
		return "real";
}