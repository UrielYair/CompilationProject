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
		new_id->numOfArguments = 0;
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
			}
		}
	}
	return everyIDWasInUse;
}

bool isFunction(char* id_name) {
	ID_Information* idToCheck = find(id_name);
	if (idToCheck != NULL)
		if (strcmp(idToCheck->functionOrVariable, "function") == 0)
			return true;
	return false;
}

void checkFunctionArguments(char* functionName, slist* receivedParameters) {
	// TODO: make function that get the nth element from the slist.
	// after that, delete the whole for loop.
	ID_Information* functionInformation = NULL;
	slist* actualParameters = NULL;
	snode *currentActualParameter, *currentReceivedParameter;
	ID_Information* currentReceivedParameterFullInformation = NULL;
	ID_Information* currentActualParameterIdInformation = NULL;
	
	functionInformation = find(functionName);

	if (functionInformation != NULL)		// Check if id of functionName is even exist.
	{
		if (!isFunction(functionName))		// Check if functionName is function.
		{
			fprintf(semanticOutput, "Can't check the arguments because %s is not a function!\n", functionName);
			return;
		}
		
		actualParameters = functionInformation->listOfArguments;

		//	In case the function declared without parameter
		//	and also no parameters were sent.
		if (actualParameters == NULL && receivedParameters == NULL)
			return;
		
		if (receivedParameters != NULL && functionInformation->numOfArguments != receivedParameters->count)
		{
			fprintf(semanticOutput, "Wrong amount of parameters in the function call (id: %s). - line: %d \n",
				functionName, getCurrentToken()->lineNumber);
			return;
		}
		else
		{
			for (int i = 0; i < functionInformation->numOfArguments; i++)
			{
				currentActualParameterIdInformation =
					getNElementInList(actualParameters, i);
				currentReceivedParameterFullInformation =
					getNElementInList(receivedParameters, i);
			
				if (!assighnmentTypeChecking(
					currentActualParameterIdInformation->ID_Type,
					currentReceivedParameterFullInformation->ID_Type))
				{
					fprintf(semanticOutput, "Type check in parameters of function call has failed (id: %s).\n", functionName);
					fprintf(semanticOutput, "Types mismatch. %s can't hold %s. parameter number: %d. - line: %d.\n",
						currentActualParameterIdInformation->ID_Type,
						currentReceivedParameterFullInformation->ID_Type,
						i+1,
						getCurrentToken()->lineNumber);
				}
				
			} // end of for loop.

			
		}
	}
	else
	{
		fprintf(semanticOutput, "Can't check the arguments of (%s) because it not declared yet! - line: %d.\n",
			functionName, getCurrentToken()->lineNumber);
		return;
	}

}

bool isAValueCanHoldBValue(ID_Information * A, ID_Information * B) {

	char* aType;
	char* bType;
	
	ID_Information* testA = find(A->name);
	ID_Information* testB = find(B->name);
	
	aType = _strdup(testA->ID_Type);
	bType = _strdup(testB->ID_Type);

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
bool assighnmentTypeChecking(char* leftType, char* rightType) {

	if (leftType == NULL || rightType == NULL)
		return false;

	if (strcmp(leftType, "error_type") == 0 || strcmp(rightType, "error_type") == 0)
	{
		return false;
	}

	if ((strcmp(leftType, "void") == 0 && strcmp(rightType, "void") == 0)
		||
		(strcmp(leftType, "integer") == 0 && strcmp(rightType, "integer") == 0)
		||
		(strcmp(leftType, "real") == 0 && strcmp(rightType, "integer") == 0)
		||
		(strcmp(leftType, "real") == 0 && strcmp(rightType, "real") == 0))
		return true;
	
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