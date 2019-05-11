#ifndef PARSER_H
#define PARSER_H

void parse_PROGRAM(FILE*);
void parse_VAR_DEFINITIONS(FILE*);
void parse_VAR_DEFINITIONS_SUFFIX(FILE*);
void parse_VAR_DEFINITION(FILE*);
void parse_TYPE(FILE*);
void parse_VARIABLES_LIST(FILE*);
void parse_VARIABLES_LIST_SUFFIX(FILE*);
void parse_VARIABLE(FILE*);
void parse_VARIABLE_SUFFIX(FILE*);
void parse_FUNC_DEFINITIONS(FILE*);
void parse_FUNC_DEFINITIONS_SUFFIX(FILE*);
void parse_FUNC_DEFINITION(FILE*);
void parse_RETURNED_TYPE(FILE*);
void parse_PARAM_DEFINITIONS(FILE*);
void parse_STATEMENTS(FILE*);
void parse_STATEMENTS_SUFFIX(FILE*);
void parse_STATEMENT(FILE*);
void parse_STATEMENT_SUFFIX(FILE*);
void parse_RETURN_SUFFIX(FILE*);
void parse_BLOCK(FILE*);
void parse_PARAMETERS_LIST(FILE*);
void parse_EXPRESSION(FILE*);


#endif // !Parser_
