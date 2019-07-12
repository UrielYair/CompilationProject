#include "ID_Information.h"
#include "Token.c"
#include "ht_hash_table.h"
#include "ht_item.c"
#include "SymbolTable.c"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


ID_Information* new_ID_Information(char* name) {

	ID_Information* new_id = (ID_Information*)malloc(sizeof(ID_Information));

	if (new_id != NULL)
	{
		strcpy(new_id->name,name);
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

bool wereAllIDsUsed()
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

bool isFunction(char* id_name) {
	ID_Information* idToCheck = find(id_name);
	if (strcmp(idToCheck->functionOrVariable, "function") == 0)
		return true;
	return false;
}

void checkFunctionArguments(char* id_name, slist * argumentsOfFunction) {

	// Check if the id is a function.
	if (!isFunction(id_name))
	{
		printf("Can't check the arguments because %s is not a function!\n", id_name);
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

	for (a = argumentdsOfIdToCheck->head->data, b = argumentsOfFunction->head->data;
		a->next != NULL && b->next != NULL;
		a = a->next, b = b->next)
	{
		ID_Information* A = (ID_Information*)a;
		ID_Information* B = (ID_Information*)b;
		if (!isAValueCanHoldBValue(A, B))
			printf("Types mismatch. %s can't hold %s.\n",A->ID_Type,B->ID_Type);
	}
}

bool isAValueCanHoldBValue(ID_Information * A, ID_Information * B) {

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

	printf("%s can not get the value of %s as a variable.\n", A->name, B->name);
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