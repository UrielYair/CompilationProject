#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "HashTable.h"
#include <stdbool.h>

typedef struct SymbolTable {

	SymbolTable* father;
	ht_hash_table* currentSymbolTable;

} SymbolTable;

typedef struct ID_Information {
	char* name;
	char* functionOrVariable;
	bool wasUsed;

	//ID_Information - variables
	char* ID_Type;
	int sizeOfArray; // (in case of array.)
	// TODO: when allocate memory check that the size of the array is not 0.


	//ID_Information - functions declaration
	char* returnedType;
	int numOfArguments;
	ID_Information** listOfArguments;

} ID_Information;


//	*******************   functions   *******************

	SymbolTable*	make_table();
	SymbolTable*	pop_table();
	ID_Information*	insert(char* id_name);
	ID_Information* lookup(char* id_name);
	ID_Information* find(char* id_name);
	void			set_id_type(ID_Information* id_entry, char* id_type);
	char*			get_id_type(ID_Information* id_entry);

	void			free_ID_info(ID_Information* id_entry);
#endif  // SYMBOL_TABLE_H