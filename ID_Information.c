#include "ID_Information.h"
#include "Token.h"
#include "ht_hash_table.h"
#include "ht_item.h"
#include "SymbolTable.h"
#include "Utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


ID_Information* new_ID_Information(char* name) {

	ID_Information* new_id = (ID_Information*)malloc(sizeof(ID_Information));

	if (new_id != NULL)
	{
		new_id->name = _strdup(name);
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
				fprintf(semanticOutput, "Variable (%s) wasn't in use in the scope that end in line: %d.\n", currentID->name, getCurrentToken()->lineNumber);
				counterOfIDsWhichNotUsed++;
			}
		}
	}

	if (everyIDWasInUse)
		fprintf(semanticOutput, "All the ID's were in used in the scope that end in line: %d.\n", getCurrentToken()->lineNumber);
	else
		fprintf(semanticOutput, "SUMMARY: %u ID's were not in use in the scope that end in line: %d.\n", counterOfIDsWhichNotUsed, getCurrentToken()->lineNumber);

	return everyIDWasInUse;
}

bool isFunction(char* id_name) {
	ID_Information* idToCheck = find(id_name);
	if (idToCheck != NULL)
		if (strcmp(idToCheck->functionOrVariable, "function") == 0)
			return true;
	return false;
}

void checkFunctionArguments(char* id_name, slist * argumentsOfFunction) {

	// Check if the id is a function.
	if (!isFunction(id_name))
	{
		fprintf(semanticOutput, "Can't check the arguments because %s is not a function!\n", id_name);
		return;
	}

	ID_Information* idToCheck = find(id_name);
	slist* argumentdsOfIdToCheck = idToCheck->listOfArguments;
	snode* a, * b;

	// Checking if the amount of argument is equal to the 
	// recieved variables in the functions call.
	int listASize, listBSize, parameterNumber=1;
	listASize = slist_get_count(argumentdsOfIdToCheck);
	listBSize = slist_get_count(argumentsOfFunction);
	if (listASize != listBSize)
	{
		fprintf(semanticOutput, "Type check in parameters of function call has failed (id: %s).\n", id_name ); 
		fprintf(semanticOutput, "The amount of parameters in the function call doesn't match the function definition. - line: %d \n",getCurrentToken()->lineNumber);
		return;
	}

	for (a = argumentdsOfIdToCheck->head->data, b = argumentsOfFunction->head->data;
		a->next != NULL && b->next != NULL;
		a = a->next, b = b->next,parameterNumber++)
	{
		ID_Information* A = (ID_Information*)a;
		ID_Information* B = (ID_Information*)b;
		if (!isAValueCanHoldBValue(A, B))
		{
			fprintf(semanticOutput, "Type check in parameters of function call has failed (id: %s).\n");
			fprintf(semanticOutput, "Types mismatch. %s can't hold %s. parameter number: %d. - line: %d.\n", 
				A->ID_Type, B->ID_Type, parameterNumber, getCurrentToken()->lineNumber);
		}
	}
}

bool isAValueCanHoldBValue(ID_Information * A, ID_Information * B) {

	char* aType;
	char* bType;
	
	aType = _strdup(find(A->name)->ID_Type);
	bType = _strdup(find(B->name)->ID_Type);

	if (isFunction(A->name))
	{
		fprintf(semanticOutput, "ID of function type can not hold variable value.\n");
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

	fprintf(semanticOutput, "%s can not get the value of %s as a variable.\n", A->name, B->name);
	return false;
}
bool checkBoundaries(int indexInArray, int sizeOfArray) {
	if (indexInArray >= 0 && indexInArray <= sizeOfArray)
		return true;
	return false;
}
bool assighnmentTypeChecking(char* leftType, char* rightType, int lineNumberWithAssighnment) {

	if (leftType == NULL || rightType == NULL)
		return false;

	if (strcmp(leftType, "error_type") == 0 || strcmp(rightType, "error_type") == 0)
	{
		fprintf(semanticOutput, "Assighnment Types error: left side type [%s] right side type [%s]. - line number: %u.\n", leftType, rightType, lineNumberWithAssighnment);
		return false;
	}

	if ((strcmp(leftType, "integer") == 0 && strcmp(rightType, "integer") == 0)
		||
		(strcmp(leftType, "real") == 0 && strcmp(rightType, "integer") == 0)
		||
		(strcmp(leftType, "real") == 0 && strcmp(rightType, "real") == 0))
		return true;

	fprintf(semanticOutput, "Assighnment Types error: left side type [%s] right side type [%s]. - line number: %u.\n", leftType, rightType, lineNumberWithAssighnment);
	return false;

}
char* arithmeticTypeChecking(char* operandA, char* operandB) {
	if (operandA==NULL || operandB ==NULL)
		return "error_type";
	if (strcmp(operandA, operandB) == 0)
		return operandA;
	if (strcmp(operandA, "error_type") == 0 || strcmp(operandB, "error_type") == 0)
		return "error_type";
	if (strcmp(operandA, "real") == 0 || strcmp(operandB, "real") == 0)
		return "real";
	return "error_type";
}