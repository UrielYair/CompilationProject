#ifndef PARSER_H
#define PARSER_H

void parse_PROGRAM(FILE* output);
void parse_VAR_DEFINITIONS(FILE* output);
void parse_VAR_DEFINITIONS_SUFFIX(FILE* output);
void parse_VAR_DEFINITION(FILE* output);
void parse_TYPE(FILE* output);
void parse_VARIABLES_LIST(FILE* output);
void parse_VARIABLES_LIST_SUFFIX(FILE* output);
void parse_VARIABLE(FILE* output);
void parse_VARIABLE_SUFFIX(FILE* output);
void parse_FUNC_DEFINITIONS(FILE* output);
void parse_FUNC_DEFINITIONS_SUFFIX(FILE* output);
void parse_FUNC_DEFINITION(FILE* output);
void parse_RETURNED_TYPE(FILE* output);
void parse_PARAM_DEFINITIONS(FILE* output);
void parse_STATEMENTS(FILE* output);
void parse_STATEMENTS_SUFFIX(FILE* output);
void parse_STATEMENT(FILE* output);
void parse_STATEMENT_SUFFIX(FILE* output);
void parse_RETURN_SUFFIX(FILE* output);
void parse_BLOCK(FILE* output);
void parse_PARAMETERS_LIST(FILE* output);
void parse_EXPRESSION(FILE* output);

#endif 
