#ifndef PARSER_H
#define PARSER_H

#include "slist.h"
#include "ID_Information.h"

void	parse_PROGRAM(FILE* output);
slist*	parse_VAR_DEFINITIONS(FILE* outputFile);
slist*	parse_VAR_DEFINITIONS_SUFFIX(FILE* outputFile);
slist*	parse_VAR_DEFINITION(FILE* outputFile);
char*	parse_TYPE(FILE* outputFile);
slist*	parse_VARIABLES_LIST(FILE* outputFile, char* id_type);
slist*	parse_VARIABLES_LIST_SUFFIX(FILE* outputFile, char* id_type);
ID_Information*	parse_VARIABLE(FILE* outputFile, bool declaring);
int		parse_VARIABLE_SUFFIX(FILE* outputFile, char* id_name);
void	parse_FUNC_DEFINITIONS(FILE* outputFile);
void	parse_FUNC_DEFINITIONS_SUFFIX(FILE* outputFile);
void	parse_FUNC_DEFINITION(FILE* outputFile);
char*	parse_RETURNED_TYPE(FILE* outputFile);
slist*	parse_PARAM_DEFINITIONS(FILE* outputFile);
char*	parse_STATEMENTS(FILE* outputFile);
char*	parse_STATEMENTS_SUFFIX(FILE* outputFile);
char*	parse_STATEMENT(FILE* outputFile);
void	parse_STATEMENT_SUFFIX(FILE* outputFile, char* id_name);
char*	parse_RETURN_SUFFIX(FILE* outputFile);
char*	parse_BLOCK(FILE* outputFile);
slist*	parse_PARAMETERS_LIST(FILE* outputFile);
char*	parse_EXPRESSION(FILE* outputFile);

void	parse_BB();
void	parse_FB();

#endif 
