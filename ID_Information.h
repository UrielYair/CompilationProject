#ifndef ID_INFORMATION_H
#define ID_INFORMATION_H

#include <stdbool.h>
#include "slist.h"

typedef struct ID_Information {

	char* name;
	char* functionOrVariable;		// function, variable
	bool wasUsed;

	//ID_Information - variables
	char* ID_Type;					// real, integer
	int sizeOfArray;				// in case of array.
	bool isArray;

	//ID_Information - functions declaration
	char* returnedType;				// real, integer, void
	int numOfArguments;
	struct slist* listOfArguments;

} ID_Information;


ID_Information* new_ID_Information(char* name);
void delete_ID_Information(ID_Information* idToDelete);
ID_Information* new_ID_Information(char* name);
bool isFunction(char* id_name);
void	checkFunctionArguments(char* id_name, slist* argumentsOfFunction);
bool	isAValueCanHoldBValue(ID_Information* A, ID_Information* B);
bool	checkBoundaries(int indexInArray, int sizeOfArray);
bool	assighnmentTypeChecking(char* leftType, char* rightType, int lineNumberWithAssighnment);
char* arithmeticTypeChecking(char* operandA, char* operandB);
bool			wereAllIDsUsed();

#endif // ID_INFORMATION_H