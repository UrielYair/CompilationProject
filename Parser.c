#include "Parser.h"
#include "Token.h"

// TODO: check for error handling.
// todo: check returned value in every match call.
// todo: fix default cases with error printing and handling. in each.
// TODO: fix nullable variables with back_token call.
// TODO: add match call to token identify.


void parse_PROGRAM()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_PROGRAM:
		parse_VAR_DEFINITIONS();
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS();
		match(TOKEN_KW_END);
		parse_FUNC_DEFINITIONS();
		break;
	}
}



void parse_VAR_DEFINITIONS()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		parse_VAR_DEFINITION();
		parse_VAR_DEFINITIONS_SUFFIX();
		break;
	}
}
void parse_VAR_DEFINITIONS_SUFFIX()
{
	// nullable
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_SEMICOLON:
	{
		Token* tokenToCheck = peekN(t, 1);
		if (tokenToCheck->kind == TOKEN_KW_INTEGER || tokenToCheck->kind == TOKEN_KW_REAL)
			parse_VAR_DEFINITIONS();
		else
			back_token();
		break;
	}
	case TOKEN_CLOSE_ROUND_BRACKETS: // EPSILON
		back_token();
		break;
	}
}



void parse_VAR_DEFINITION()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_INTEGER:
	case TOKEN_KW_REAL:
		parse_TYPE();
		parse_VARIABLES_LIST();
		break;

	}

}
void parse_TYPE()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		break;

	}
}

void parse_VARIABLES_LIST()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_ID:
		parse_VARIABLE();
		parse_VARIABLES_LIST_SUFFIX();
		break;

	}
}


void parse_VARIABLES_LIST_SUFFIX()
{
	// nullable

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_SEMICOLON:
		parse_VARIABLE();
		parse_VARIABLES_LIST_SUFFIX();
		break;



	}
}


void parse_VARIABLE()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_ID:
		parse_VARIABLE_SUFFIX();
		break;
	}
}

void parse_VARIABLE_SUFFIX()
{
	//nullable
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_OPEN_SQUARE_BRACKETS:
		match(TOKEN_INT_NUMBER);
		match(TOKEN_CLOSE_SQUARE_BRACKETS);
		break;

	case TOKEN_SEMICOLON:
	case TOKEN_COMMA:
	case TOKEN_OP_EQUAL:
	case TOKEN_CLOSE_ROUND_BRACKETS:
		back_token();
		break;


	}
}
void parse_FUNC_DEFINITIONS()
{

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_VOID:
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		parse_FUNC_DEFINITION();
		parse_FUNC_DEFINITIONS_SUFFIX();
		break;
	}


}
void parse_FUNC_DEFINITIONS_SUFFIX()
{
	// nullable
	// TODO: keep checking follow from here and add cases.
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_VOID:
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		parse_FUNC_DEFINITION();
		parse_FUNC_DEFINITIONS_SUFFIX();
		break;

	}

}
void parse_FUNC_DEFINITION()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_VOID:
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		parse_RETURNED_TYPE();
		match(TOKEN_ID);
		match(TOKEN_OPEN_ROUND_BRACKETS);
		parse_PARAM_DEFINITIONS();
		match(TOKEN_CLOSE_ROUND_BRACKETS);
		parse_BLOCK();
		break;
	}
}
void parse_RETURNED_TYPE()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_VOID:
		break;
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		parse_TYPE();
		break;
	}
}

void parse_PARAM_DEFINITIONS()
{
	// nullable

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		parse_VAR_DEFINITIONS();
		break;

	}
}

void parse_STATEMENTS()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_RETURN:
	case TOKEN_ID:
	case TOKEN_OPEN_CURLY_BRACKETS:
		parse_STATEMENT();
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS_SUFFIX();
		break;
	}
}
void parse_STATEMENTS_SUFFIX()
{
	// nullable

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_RETURN:
	case TOKEN_ID:
	case TOKEN_OPEN_CURLY_BRACKETS:
		parse_STATEMENTS();
		break;
	}
}


void parse_STATEMENT()
{
	Token* t = next_token();
	switch (t->kind)
	{

	case TOKEN_KW_RETURN:
		parse_RETURN_SUFFIX();
		break;

	case TOKEN_OPEN_CURLY_BRACKETS:
		parse_BLOCK();
		break;

	case TOKEN_ID:
		parse_STATEMENT_SUFFIX();
		break;


	}
}

void parse_STATEMENT_SUFFIX() 
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_OPEN_ROUND_BRACKETS:
		parse_PARAMETERS_LIST();
		match(TOKEN_CLOSE_ROUND_BRACKETS);
		break;

	case TOKEN_OPEN_SQUARE_BRACKETS:
	case TOKEN_OP_EQUAL:
		parse_VARIABLE_SUFFIX();
		match(TOKEN_OP_EQUAL);
		parse_EXPRESSION();
		break;
	}
	
}
void parse_RETURN_SUFFIX()
{
	//nullable
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_INT_NUMBER:
	case TOKEN_REAL_NUMBER:
	case TOKEN_ID:
		parse_EXPRESSION();
		break;
	}
}
void parse_BLOCK()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_OPEN_CURLY_BRACKETS:
		parse_VAR_DEFINITIONS();
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS();
		match(TOKEN_CLOSE_CURLY_BRACKETS);
		break;
	}
}
void parse_PARAMETERS_LIST()
{
	//nullable
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_ID:
		parse_VARIABLES_LIST();
		break;
	}
}
void parse_EXPRESSION()
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_INT_NUMBER:
	case TOKEN_REAL_NUMBER:
		break;
	case TOKEN_ID:
		t = next_token();
		if (t->kind == TOKEN_OP_EQUAL)
		{
			t = back_token();
			match(TOKEN_OP_EQUAL);
			parse_EXPRESSION();
		}
		else
		{
			t= back_token();
			parse_VARIABLE();
		}
		break;
	}
}
