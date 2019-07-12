#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


#include "ht_hash_table.h"


typedef struct SymbolTable {
	struct SymbolTable* father;
	ht_hash_table* currentSymbolTable;
} SymbolTable;


//	*******************   functions   *******************

	SymbolTable*	make_table();
	SymbolTable*	pop_table();
	ID_Information*	insert(char* id_name);
	ID_Information* lookup(char* id_name);
	ID_Information* find(char* id_name);
	void			set_id_info(ID_Information* id_entry, char* whatToUpdate, void* value);
	char*			get_id_type(ID_Information* id_entry);
	
		
	bool isIDExistInSymbolTable(char* id_name);
	void checkIfIDAlreadyDeclared(char* id_name);
	
#endif  // SYMBOL_TABLE_H