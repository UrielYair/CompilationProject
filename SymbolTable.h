#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "HashTable.h"
#include "IDInfoLinkedList.h"
#include <stdbool.h>

typedef struct SymbolTable {

	SymbolTable* father;
	ht_hash_table* currentSymbolTable;

} SymbolTable;

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
	struct IDInfoLinkNode* listOfArguments;

} ID_Information;

ID_Information* new_ID_Information(char* name);
void delete_ID_Information(ID_Information* idToDelete);


//	*******************   functions   *******************

	SymbolTable*	make_table();
	SymbolTable*	pop_table();
	ID_Information*	insert(char* id_name);
	ID_Information* lookup(char* id_name);
	ID_Information* find(char* id_name);
	void			set_id_info(ID_Information* id_entry, char* whatToUpdate, void* value);
	char*			get_id_type(ID_Information* id_entry);
	
	ID_Information* new_ID_Information(char* name);
	void delete_ID_Information(ID_Information* idToDelete);
	bool			wereAllIDsUsed();
	
	bool isIDExistInSymbolTable(char* id_name);
	void checkIfIDAlreadyDeclared(char* id_name);
	bool isFunction(char* id_name);

#endif  // SYMBOL_TABLE_H