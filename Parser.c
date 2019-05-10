#include "Parser.h"
#include "Token.h"

//TODO: add messeges printing about errors and rules in use.

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
	default:
		while (t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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
	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}
	
}
void parse_VAR_DEFINITIONS_SUFFIX()
{
	// nullable - done
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_SEMICOLON:
	{
		Token* tokenToCheck = peekN(t, 1); // looking to check what is the kind of the next token to decise how to act next.
		if (tokenToCheck->kind == TOKEN_KW_INTEGER || tokenToCheck->kind == TOKEN_KW_REAL)
			parse_VAR_DEFINITIONS();
		else
			back_token();
		break;
	}
	case TOKEN_CLOSE_ROUND_BRACKETS: // EPSILON
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
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

	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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
	
	default:
		while (
			t->kind != TOKEN_ID
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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

	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_VARIABLES_LIST_SUFFIX()
{
	// nullable - done

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_COMMA:
		parse_VARIABLE();
		parse_VARIABLES_LIST_SUFFIX();
		break;

	case TOKEN_SEMICOLON:
	case TOKEN_CLOSE_ROUND_BRACKETS:
		back_token();
		break;
		
	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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
	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS && t->kind != TOKEN_COMMA
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_VARIABLE_SUFFIX()
{
	//nullable - done
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

		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS && t->kind != TOKEN_COMMA && t->kind != TOKEN_OP_EQUAL
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
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
	default:
		while (
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}


}
void parse_FUNC_DEFINITIONS_SUFFIX()
{
	// nullable - done
	
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_VOID:
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		parse_FUNC_DEFINITION();
		parse_FUNC_DEFINITIONS_SUFFIX();
		break;

	case TOKEN_END_OF_FILE:
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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

	default:
		while (
			t->kind != TOKEN_KW_VOID && t->kind != TOKEN_KW_REAL && t->kind != TOKEN_KW_INTEGER 
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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
	default:
		while (
			t->kind != TOKEN_ID
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_PARAM_DEFINITIONS()
{
	// nullable - done

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		parse_VAR_DEFINITIONS();
		break;
	case TOKEN_CLOSE_ROUND_BRACKETS:
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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
	default:
		while (
			t->kind != TOKEN_KW_END && t->kind != TOKEN_CLOSE_CURLY_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_STATEMENTS_SUFFIX()
{
	// nullable -done

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_RETURN:
	case TOKEN_ID:
	case TOKEN_OPEN_CURLY_BRACKETS:
		parse_STATEMENTS();
		break;

	case TOKEN_KW_END:
	case TOKEN_CLOSE_CURLY_BRACKETS:
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_KW_END && t->kind != TOKEN_CLOSE_CURLY_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_STATEMENT()
{
	Token* t = next_token();
	switch (t->kind)
	{

	case TOKEN_OPEN_CURLY_BRACKETS:
		parse_BLOCK();
		break;

	case TOKEN_KW_RETURN:
		parse_RETURN_SUFFIX();
		break;

	case TOKEN_ID:
		parse_STATEMENT_SUFFIX();
		break;

	default:
		while (
			t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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

	default:
		while (
			t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}
	
}
void parse_RETURN_SUFFIX()
{
	//nullable - done
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_INT_NUMBER:
	case TOKEN_REAL_NUMBER:
	case TOKEN_ID:
		parse_EXPRESSION();
		break;
	case TOKEN_SEMICOLON:
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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
	default:
		while (
			t->kind != TOKEN_KW_VOID && t->kind != TOKEN_KW_REAL && t->kind != TOKEN_KW_INTEGER && t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
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
	case TOKEN_CLOSE_ROUND_BRACKETS:
		back_token();
		break;
	default:
		while (
			t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_EXPRESSION()
{
	Token* t = next_token();
	Token* tokenToCheck;

	switch (t->kind)
	{
	case TOKEN_INT_NUMBER:
	case TOKEN_REAL_NUMBER:
		break;
	case TOKEN_ID:
		
		tokenToCheck = peekN(t, 1); // looking to check what is the kind of the next token to decise how to act next.
		
		if (tokenToCheck->kind == TOKEN_OP_EQUAL)
		{
			match(TOKEN_OP_EQUAL);
			parse_EXPRESSION();
		}
		else
		{
			parse_VARIABLE();
		}
		break;
	default:
		while (
			t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}
}