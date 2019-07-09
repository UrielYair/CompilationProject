#ifndef PARSER_H
#define PARSER_H

void			parse_PROGRAM(FILE* output);
IDInfoLinkNode* parse_VAR_DEFINITIONS(FILE* outputFile);
IDInfoLinkNode* parse_VAR_DEFINITIONS_SUFFIX(FILE* outputFile);
IDInfoLinkNode* parse_VAR_DEFINITION(FILE* outputFile);
char*			parse_TYPE(FILE* outputFile);
IDInfoLinkNode* parse_VARIABLES_LIST(FILE* outputFile, char* id_type);
IDInfoLinkNode* parse_VARIABLES_LIST_SUFFIX(FILE* outputFile, char* id_type);
ID_Information* parse_VARIABLE(FILE* outputFile);
int				parse_VARIABLE_SUFFIX(FILE* outputFile, char* id_name);
void			parse_FUNC_DEFINITIONS(FILE* outputFile);
void			parse_FUNC_DEFINITIONS_SUFFIX(FILE* outputFile);
void			parse_FUNC_DEFINITION(FILE* outputFile);
char*			parse_RETURNED_TYPE(FILE* outputFile);
IDInfoLinkNode* parse_PARAM_DEFINITIONS(FILE* outputFile);
char*			parse_STATEMENTS(FILE* outputFile);
char*			parse_STATEMENTS_SUFFIX(FILE* outputFile);
char*			parse_STATEMENT(FILE* outputFile);
void			parse_STATEMENT_SUFFIX(FILE* outputFile, char* id_name);
char*			parse_RETURN_SUFFIX(FILE* outputFile);
char*			parse_BLOCK(FILE* outputFile);
IDInfoLinkNode* parse_PARAMETERS_LIST(FILE* outputFile);
char*			parse_EXPRESSION(FILE* outputFile);

void			getAllFuctionDeclared(FILE* outputFile);

void			parse_BB();
void			parse_FB();

#endif 
