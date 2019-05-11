#include "Parser.h"
#include "Token.h"


void parse_PROGRAM(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_PROGRAM:
		fprintf(outputFile, "Rule(PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS)\n");
		parse_VAR_DEFINITIONS(outputFile);
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS(outputFile);
		match(TOKEN_KW_END);
		parse_FUNC_DEFINITIONS(outputFile);
		break;
	default:
		while (t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n", 
				"EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_VAR_DEFINITIONS(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_SUFFIX)\n");
		parse_VAR_DEFINITION(outputFile);
		parse_VAR_DEFINITIONS_SUFFIX(outputFile);
		break;
	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; ) EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
	
}
void parse_VAR_DEFINITIONS_SUFFIX(FILE* outputFile)
{
	// nullable - done
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_SEMICOLON:
	{
		Token* tokenToCheck = peekN(t, 1); // looking to check what is the kind of the next token to decise how to act next.
		if (tokenToCheck->kind == TOKEN_KW_INTEGER || tokenToCheck->kind == TOKEN_KW_REAL)
		{
			fprintf(outputFile, "Rule(VAR_DEFINITIONS_SUFFIX  -> ; VAR_DEFINITIONS)\n");
			parse_VAR_DEFINITIONS(outputFile);
		}
		else
		{
			fprintf(outputFile, "Rule(VAR_DEFINITIONS_SUFFIX->ε)\n");
			back_token();
		}
		break;
	}
	case TOKEN_CLOSE_ROUND_BRACKETS: // EPSILON
		fprintf(outputFile, "Rule(VAR_DEFINITIONS_SUFFIX->ε)\n");
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; ) EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_VAR_DEFINITION(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_INTEGER:
	case TOKEN_KW_REAL:
		fprintf(outputFile, "Rule(VAR_DEFINITION ->  TYPE  VARIABLES_LIST)\n");
		parse_TYPE(outputFile);
		parse_VARIABLES_LIST(outputFile);
		break;

	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; ) EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_TYPE(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_REAL:
		fprintf(outputFile, "Rule(TYPE ->  real)\n");
		break;
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(TYPE ->  integer)\n");
		break;
	
	default:
		while (
			t->kind != TOKEN_ID
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"ID EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_VARIABLES_LIST(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_ID:
		fprintf(outputFile, "Rule(VARIABLES_LIST ->  VARIABLE   VARIABLES_LIST_SUFFIX)\n");
		parse_VARIABLE(outputFile);
		parse_VARIABLES_LIST_SUFFIX(outputFile);
		break;

	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; ) EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_VARIABLES_LIST_SUFFIX(FILE* outputFile)
{
	// nullable - done

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_COMMA:
		fprintf(outputFile, "Rule(VARIABLES_LIST_SUFFIX->  , VARIABLE   VARIABLES_LIST_SUFFIX)\n");
		parse_VARIABLE(outputFile);
		parse_VARIABLES_LIST_SUFFIX(outputFile);
		break;

	case TOKEN_SEMICOLON:
	case TOKEN_CLOSE_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(VARIABLES_LIST_SUFFIX->  ε)\n");
		back_token();
		break;
		
	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; ) EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_VARIABLE(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_ID:
		fprintf(outputFile, "Rule(VARIABLE ->  id VARIABLE_SUFFIX)\n");
		parse_VARIABLE_SUFFIX(outputFile);
		break;
	default:
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS && t->kind != TOKEN_COMMA
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; ) , EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_VARIABLE_SUFFIX(FILE* outputFile)
{
	//nullable - done
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_OPEN_SQUARE_BRACKETS:
		fprintf(outputFile, "Rule(VARIABLE_SUFFIX-> [ int_number ])\n");
		match(TOKEN_INT_NUMBER);
		match(TOKEN_CLOSE_SQUARE_BRACKETS);
		break;

	case TOKEN_SEMICOLON:
	case TOKEN_COMMA:
	case TOKEN_ARITHMETIC_ASSIGNMENT:
	case TOKEN_CLOSE_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(VARIABLE_SUFFIX-> ε)\n");
		back_token();
		break;

		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS && t->kind != TOKEN_COMMA && t->kind != TOKEN_ARITHMETIC_ASSIGNMENT
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; ) , = EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;

	}
}
void parse_FUNC_DEFINITIONS(FILE* outputFile)
{

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_VOID:
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_SUFFIX )\n");
		parse_FUNC_DEFINITION(outputFile);
		parse_FUNC_DEFINITIONS_SUFFIX(outputFile);
		break;
	default:
		while (
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}


}
void parse_FUNC_DEFINITIONS_SUFFIX(FILE* outputFile)
{
	// nullable - done
	
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_VOID:
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(FUNC_DEFINITIONS_SUFFIX-> FUNC_DEFINITION FUNC_DEFINITIONS_SUFFIX)\n");
		parse_FUNC_DEFINITION(outputFile);
		parse_FUNC_DEFINITIONS_SUFFIX(outputFile);
		break;

	case TOKEN_END_OF_FILE:
		fprintf(outputFile, "Rule(FUNC_DEFINITIONS_SUFFIX-> ε)\n");
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}

}
void parse_FUNC_DEFINITION(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_VOID:
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(FUNC_DEFINITION -> RETURNED_TYPE id ( PARAM_DEFINITIONS ) BLOCK)\n");
		parse_RETURNED_TYPE(outputFile);
		match(TOKEN_ID);
		match(TOKEN_OPEN_ROUND_BRACKETS);
		parse_PARAM_DEFINITIONS(outputFile);
		match(TOKEN_CLOSE_ROUND_BRACKETS);
		parse_BLOCK(outputFile);
		break;

	default:
		while (
			t->kind != TOKEN_KW_VOID && t->kind != TOKEN_KW_REAL && t->kind != TOKEN_KW_INTEGER 
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"void real integer EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_RETURNED_TYPE(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_VOID:
		fprintf(outputFile, "Rule(RETURNED_TYPE ->  void)\n");
		break;
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(RETURNED_TYPE ->  TYPE)\n");
		parse_TYPE(outputFile);
		break;
	default:
		while (
			t->kind != TOKEN_ID
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"ID EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_PARAM_DEFINITIONS(FILE* outputFile)
{
	// nullable - done

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(PARAM_DEFINITIONS->  VAR_DEFINITIONS)\n");
		parse_VAR_DEFINITIONS(outputFile);
		break;
	case TOKEN_CLOSE_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(PARAM_DEFINITIONS-> ε)\n");
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				") EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_STATEMENTS(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_RETURN:
	case TOKEN_ID:
	case TOKEN_OPEN_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENTS -> STATEMENT ; STATEMENTS_SUFFIX)\n");
		parse_STATEMENT(outputFile);
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS_SUFFIX(outputFile);
		break;
	default:
		while (
			t->kind != TOKEN_KW_END && t->kind != TOKEN_CLOSE_CURLY_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"end } EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_STATEMENTS_SUFFIX(FILE* outputFile)
{
	// nullable -done

	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_KW_RETURN:
	case TOKEN_ID:
	case TOKEN_OPEN_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENTS_SUFFIX-> STATEMENTS)\n");
		parse_STATEMENTS(outputFile);
		break;

	case TOKEN_KW_END:
	case TOKEN_CLOSE_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENTS_SUFFIX-> ε)\n");
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_KW_END && t->kind != TOKEN_CLOSE_CURLY_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"end } EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_STATEMENT(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{

	case TOKEN_OPEN_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENT ->  BLOCK)\n");
		parse_BLOCK(outputFile);
		break;

	case TOKEN_KW_RETURN:
		fprintf(outputFile, "Rule(STATEMENT ->  return RETURN_SUFFIX)\n");
		parse_RETURN_SUFFIX(outputFile);
		break;

	case TOKEN_ID:
		fprintf(outputFile, "Rule(STATEMENT ->  id STATEMENT_SUFFIX)\n");
		parse_STATEMENT_SUFFIX(outputFile);
		break;

	default:
		while (
			t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_STATEMENT_SUFFIX(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_OPEN_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENT_SUFFIX -> (PARAMETERS_LIST))\n");
		parse_PARAMETERS_LIST(outputFile);
		match(TOKEN_CLOSE_ROUND_BRACKETS);
		break;

	case TOKEN_OPEN_SQUARE_BRACKETS:
	case TOKEN_ARITHMETIC_ASSIGNMENT:
		fprintf(outputFile, "Rule(STATEMENT_SUFFIX -> VARIABLE_SUFFIX = EXPRESSION)\n");
		parse_VARIABLE_SUFFIX(outputFile);
		match(TOKEN_ARITHMETIC_ASSIGNMENT);
		parse_EXPRESSION(outputFile);
		break;

	default:
		while (
			t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
	
}
void parse_RETURN_SUFFIX(FILE* outputFile)
{
	//nullable - done
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_INT_NUMBER:
	case TOKEN_REAL_NUMBER:
	case TOKEN_ID:
		fprintf(outputFile, "Rule(RETURN_SUFFIX ->  EXPRESSION)\n");
		parse_EXPRESSION(outputFile);
		break;
	case TOKEN_SEMICOLON:
		fprintf(outputFile, "Rule(RETURN_SUFFIX -> ε)\n");
		back_token();
		break;

	default:
		while (
			t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_BLOCK(FILE* outputFile)
{
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_OPEN_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(BLOCK -> { VAR_DEFINITIONS; STATEMENTS })\n");
		parse_VAR_DEFINITIONS(outputFile);
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS(outputFile);
		match(TOKEN_CLOSE_CURLY_BRACKETS);
		break;
	default:
		while (
			t->kind != TOKEN_KW_VOID && t->kind != TOKEN_KW_REAL && t->kind != TOKEN_KW_INTEGER && t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"void real integer ; EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_PARAMETERS_LIST(FILE* outputFile)
{
	//nullable - done
	Token* t = next_token();
	switch (t->kind)
	{
	case TOKEN_ID:
		fprintf(outputFile, "Rule(PARAMETERS_LIST ->  VARIABLES_LIST)\n");
		parse_VARIABLES_LIST(outputFile);
		break;
	case TOKEN_CLOSE_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(PARAMETERS_LIST ->  ε)\n");
		back_token();
		break;
	default:
		while (
			t->kind != TOKEN_CLOSE_ROUND_BRACKETS
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				") EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}
void parse_EXPRESSION(FILE* outputFile)
{
	Token* t = next_token();
	Token* tokenToCheck;

	switch (t->kind)
	{
	case TOKEN_INT_NUMBER:
		fprintf(outputFile, "Rule(EXPRESSION ->  int_number)\n");
		break;
	case TOKEN_REAL_NUMBER:
		fprintf(outputFile, "Rule(EXPRESSION ->  real_number)\n");
		break;
	case TOKEN_ID:
		
		tokenToCheck = peekN(t, 1); // looking to check what is the kind of the next token to decise how to act next.
		
		if (tokenToCheck->kind == TOKEN_ARITHMETIC_ASSIGNMENT)
		{
			fprintf(outputFile, "Rule(EXPRESSION ->  id ar_op EXPRESSION)\n");
			match(TOKEN_ARITHMETIC_ASSIGNMENT);
			parse_EXPRESSION(outputFile);
		}
		else
		{
			fprintf(outputFile, "Rule(EXPRESSION ->  VARIABLE)\n");
			parse_VARIABLE(outputFile);
		}
		break;
	default:
		while (
			t->kind != TOKEN_SEMICOLON
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
				"; EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
			t = next_token();
		}
		back_token();
		break;
	}
}