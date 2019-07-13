#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


#include "ht_hash_table.h"
#include <stdbool.h>

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
	
	void			set_id_info_pointer(ID_Information* id_entry, char* whatToUpdate, void* value);
	void			set_id_info_boolean(ID_Information* id_entry, char* whatToUpdate, bool value);
	void			set_id_info_integer(ID_Information* id_entry, char* whatToUpdate, int value);
	char*			get_id_type(ID_Information* id_entry);
	
		
	bool isIDExistInSymbolTable(char* id_name);
	void checkIfIDAlreadyDeclared(char* id_name);
	
#endif  // SYMBOL_TABLE_H