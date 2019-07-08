#include "Token.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "IDInfoLinkedList.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

// TODO: after any match(TOKEN_ID); add insert() and validation.

void			parse_PROGRAM				(FILE* outputFile)
{
	Token* t = next_token();
	parse_BB();

	switch (t->kind)
	{
	case TOKEN_KW_PROGRAM:
		fprintf(outputFile, "Rule(PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS)\n");
		
		/*
			In order to get all the functions declared in the global scope,
			The program will skip right to function declaration area in the input.
			and then save them in the symbols table.
			After that, the program will get back to the first token after the keyword "program".
		*/ 
		getAllFuctionDeclared(outputFile);
		// The scanner is now on the "program" token again.

		parse_VAR_DEFINITIONS(outputFile);
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS(outputFile);
		match(TOKEN_KW_END);
		// parse_FUNC_DEFINITIONS(outputFile);   // already parsed in the begining.
		
		// Skipping function declarations area:
		while (t->kind != TOKEN_END_OF_FILE)
			t = next_token();
		back_token();

		break;
	default:
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"program", t->lineNumber, tokenToString(t->kind), t->lexeme);
		while (t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}

	parse_FB();
}

/* This function will return linked list of all the ID's that discovered durring derivation of the input.*/
IDInfoLinkNode* parse_VAR_DEFINITIONS		(FILE* outputFile)
{
	IDInfoLinkNode* headOfIdsList = NULL;
	IDInfoLinkNode* tailOfIDsList = NULL;
	Token* t = peekN(getCurrentToken(), 1);

	switch (t->kind)
	{
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_SUFFIX)\n");
		headOfIdsList = parse_VAR_DEFINITION(outputFile);			// parse_VAR_DEFINITION() will return only one ID_Information.
		tailOfIDsList = parse_VAR_DEFINITIONS_SUFFIX(outputFile);	// parse_VAR_DEFINITIONS_SUFFIX() will return list of ID's with their info.
		if (tailOfIDsList != NULL)
			headOfIdsList = listsConcat(headOfIdsList, tailOfIDsList);
		break;
	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"real integer", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return headOfIdsList;
}
IDInfoLinkNode* parse_VAR_DEFINITIONS_SUFFIX(FILE* outputFile)
{
	// nullable - done
	IDInfoLinkNode* listOfIDsToReturn = NULL;
	Token* t = peekN(getCurrentToken(), 1);
	
	switch (t->kind)
	{
	case TOKEN_SEMICOLON:
	{
		Token* tokenToCheck = peekN(t, 2); // looking to check what is the kind of the next token to decise how to act next.
		if (tokenToCheck->kind == TOKEN_KW_INTEGER || tokenToCheck->kind == TOKEN_KW_REAL)
		{
			fprintf(outputFile, "Rule(VAR_DEFINITIONS_SUFFIX  -> ; VAR_DEFINITIONS)\n");
			match(TOKEN_SEMICOLON);
			listOfIDsToReturn = parse_VAR_DEFINITIONS(outputFile);
		}
		else
		{
			fprintf(outputFile, "Rule(VAR_DEFINITIONS_SUFFIX->EPSILON)\n");
		}
		break;
	}
	case TOKEN_CLOSE_ROUND_BRACKETS: // EPSILON
		fprintf(outputFile, "Rule(VAR_DEFINITIONS_SUFFIX->EPSILON)\n");
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"; )", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return listOfIDsToReturn;
}
/* This function will return list of ID's of the same type. */
IDInfoLinkNode* parse_VAR_DEFINITION		(FILE* outputFile)
{
	IDInfoLinkNode* listOfIDs = NULL;
	Token* t = peekN(getCurrentToken(), 1);

	switch (t->kind)
	{
	case TOKEN_KW_INTEGER:
	case TOKEN_KW_REAL:
		fprintf(outputFile, "Rule(VAR_DEFINITION ->  TYPE  VARIABLES_LIST)\n");
		char* id_type = parse_TYPE(outputFile);
		listOfIDs = parse_VARIABLES_LIST(outputFile, id_type);
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"real integer", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return listOfIDs;
}
char*			parse_TYPE					(FILE* outputFile)
{
	char* typeOfID = NULL;		// in order to update attribute of the ID.
	Token* t = peekN(getCurrentToken(), 1);
	
	switch (t->kind)
	{
	case TOKEN_KW_REAL:
		fprintf(outputFile, "Rule(TYPE ->  real)\n");
		typeOfID = strdup("real"); 
		match(TOKEN_KW_REAL);
		break;
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(TYPE ->  integer)\n");
		typeOfID = strdup("integer");
		match(TOKEN_KW_INTEGER);
		break;
	
	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"real integer", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return typeOfID;
}
IDInfoLinkNode* parse_VARIABLES_LIST		(FILE* outputFile, char* id_type)
{
	IDInfoLinkNode* headOfIDsList = NULL;
	IDInfoLinkNode* tailOfIDsList = NULL;
	ID_Information* headNode = NULL;
	Token* t = peekN(getCurrentToken(), 1);
	
	switch (t->kind)
	{
	case TOKEN_ID:
		fprintf(outputFile, "Rule(VARIABLES_LIST ->  VARIABLE   VARIABLES_LIST_SUFFIX)\n");
		headNode = parse_VARIABLE(outputFile, id_type);
		headOfIDsList = makeLink(headNode);
		tailOfIDsList = parse_VARIABLES_LIST_SUFFIX(outputFile, id_type);
		if (tailOfIDsList != NULL)
			headOfIDsList = listsConcat(headOfIDsList, tailOfIDsList);
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"id", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return headOfIDsList;
}
IDInfoLinkNode* parse_VARIABLES_LIST_SUFFIX	(FILE* outputFile, char* id_type)
{
	//TODO: check if reverse list is needed.
	// nullable - done
	Token* t = peekN(getCurrentToken(), 1);
	IDInfoLinkNode* headOfIDsList = NULL;
	IDInfoLinkNode* tailOfIDsList = NULL;
	ID_Information* IDInfo = NULL;

	switch (t->kind)
	{
	case TOKEN_COMMA:
		fprintf(outputFile, "Rule(VARIABLES_LIST_SUFFIX->  , VARIABLE   VARIABLES_LIST_SUFFIX)\n");
		match(TOKEN_COMMA);
		IDInfo = parse_VARIABLE(outputFile, id_type);
		headOfIDsList = makeLink(IDInfo);
		tailOfIDsList = parse_VARIABLES_LIST_SUFFIX(outputFile, id_type);
		if (tailOfIDsList != NULL)
			headOfIDsList = listsConcat(headOfIDsList, tailOfIDsList);
		break;

	case TOKEN_SEMICOLON:
	case TOKEN_CLOSE_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(VARIABLES_LIST_SUFFIX->  EPSILON)\n");
		break;
		
	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			", ; )", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return headOfIDsList;
}
ID_Information* parse_VARIABLE				(FILE* outputFile)
{
	ID_Information* new_id = NULL;
	char* id_name;
	Token* t = peekN(getCurrentToken(), 1);

	switch (t->kind)
	{
	case TOKEN_ID:
		fprintf(outputFile, "Rule(VARIABLE ->  id VARIABLE_SUFFIX)\n");
		match(TOKEN_ID);

		id_name = getIdLexeme();
		insert(id_name);	// will automatically check if alraedy exist in the symbol table or not.
		// TODO: update attribute of id to be of id_type that get from the argument.
		
		parse_VARIABLE_SUFFIX(outputFile, id_name);		// if parse_VARIABLE_SUFFIX() will find that the id is an 
														// array, it will update the entry in the symbol table.
		new_id = find(id_name);
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"id", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return new_id;
}
char*			parse_VARIABLE_SUFFIX		(FILE* outputFile, char* id_name)
{
	//nullable - done
	Token* t = peekN(getCurrentToken(), 1);
	char* variableType = NULL;

	switch (t->kind)
	{
	case TOKEN_OPEN_SQUARE_BRACKETS:
		fprintf(outputFile, "Rule(VARIABLE_SUFFIX-> [ int_number ])\n");
		match(TOKEN_OPEN_SQUARE_BRACKETS);
		if (match(TOKEN_INT_NUMBER))
			printf("hi!");	// delete it after implementing the TODO.
			// TODO: set the id to be an array and set its size, 
			//       check that the size makes sense and not negative. 
			//		 (if the id is already an array, validate the index to be in the baundaries.)
		match(TOKEN_CLOSE_SQUARE_BRACKETS);
		variableType = get_id_type(find(getIdLexeme(id_name))); // TODO: check.
		break;

	case TOKEN_SEMICOLON:
	case TOKEN_COMMA:
	case TOKEN_ARITHMETIC_ASSIGNMENT:
	case TOKEN_CLOSE_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(VARIABLE_SUFFIX-> EPSILON)\n");
		break;
	
	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"[ ; , = )", t->lineNumber, tokenToString(t->kind), t->lexeme);
		while (
			t->kind != TOKEN_SEMICOLON && t->kind != TOKEN_CLOSE_ROUND_BRACKETS && t->kind != TOKEN_COMMA && t->kind != TOKEN_ARITHMETIC_ASSIGNMENT
			&&
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}

}
void			parse_FUNC_DEFINITIONS		(FILE* outputFile)
{
	Token* t = peekN(getCurrentToken(), 1);
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
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"void real integer", t->lineNumber, tokenToString(t->kind), t->lexeme);
		while (
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}


}
void			parse_FUNC_DEFINITIONS_SUFFIX(FILE* outputFile)
{
	// nullable - done
	
	Token* t = peekN(getCurrentToken(), 1);
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
		fprintf(outputFile, "Rule(FUNC_DEFINITIONS_SUFFIX-> EPSILON)\n");
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"void real integer EOF", t->lineNumber, tokenToString(t->kind), t->lexeme);
		while (
			t->kind != TOKEN_END_OF_FILE)
		{
			t = next_token();
		}
		back_token();
		break;
	}

}
void			parse_FUNC_DEFINITION		(FILE* outputFile)
{
	Token* t = peekN(getCurrentToken(), 1);
	char* id_type = NULL;
	IDInfoLinkNode* argumentsOfFunction = NULL;
	char* returnedTypeOfBlock = NULL;

	switch (t->kind)
	{
	case TOKEN_KW_VOID:
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(FUNC_DEFINITION -> RETURNED_TYPE id ( PARAM_DEFINITIONS ) BLOCK)\n");
		id_type = parse_RETURNED_TYPE(outputFile);
		match(TOKEN_ID);
		// TODO: insert and validation.
		// TODO: set_id with id_type and make it function type.
		match(TOKEN_OPEN_ROUND_BRACKETS);
		argumentsOfFunction = parse_PARAM_DEFINITIONS(outputFile);
		// TODO: set_id with list of arguments.
		match(TOKEN_CLOSE_ROUND_BRACKETS);
		returnedTypeOfBlock = parse_BLOCK(outputFile);
		if (strcmp(id_type, returnedTypeOfBlock) != 0)
			printf("not good!"); // TODO: change the error messege to a meaningful one
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"void real integer", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
char*			parse_RETURNED_TYPE			(FILE* outputFile)
{
	Token* t = peekN(getCurrentToken(), 1);
	char* id_type = NULL;

	switch (t->kind)
	{
	case TOKEN_KW_VOID:
		fprintf(outputFile, "Rule(RETURNED_TYPE ->  void)\n");
		match(TOKEN_KW_VOID);
		id_type = strdup("void");
		break;

	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(RETURNED_TYPE ->  TYPE)\n");
		id_type = parse_TYPE(outputFile);
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"void real integer", t->lineNumber, tokenToString(t->kind), t->lexeme);
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

	return id_type;
}
IDInfoLinkNode* parse_PARAM_DEFINITIONS		(FILE* outputFile)
{
	// nullable - done
	Token* t = peekN(getCurrentToken(), 1);
	IDInfoLinkNode* arguments = NULL;

	switch (t->kind)
	{
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:
		fprintf(outputFile, "Rule(PARAM_DEFINITIONS->  VAR_DEFINITIONS)\n");
		arguments = parse_VAR_DEFINITIONS(outputFile);
		break;

	case TOKEN_CLOSE_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(PARAM_DEFINITIONS-> EPSILON)\n");
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"real integer )", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return arguments;
}
char*			parse_STATEMENTS			(FILE* outputFile)
{
	Token* t = peekN(getCurrentToken(), 1);
	char* returnedTypeOfStatement = NULL;
	char* returnedTypeOfStatementSuffix = NULL;

	switch (t->kind)
	{
	case TOKEN_KW_RETURN:
	case TOKEN_ID:
	case TOKEN_OPEN_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENTS -> STATEMENT ; STATEMENTS_SUFFIX)\n");
		returnedTypeOfStatement = parse_STATEMENT(outputFile);
		match(TOKEN_SEMICOLON);
		returnedTypeOfStatementSuffix = parse_STATEMENTS_SUFFIX(outputFile);
		if (returnedTypeOfStatementSuffix != NULL)
			strcpy(returnedTypeOfStatement, returnedTypeOfStatementSuffix);
		else
			strcpy(returnedTypeOfStatement, "void");
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"return id {", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
char*			parse_STATEMENTS_SUFFIX		(FILE* outputFile)
{
	// nullable -done
	Token* t = peekN(getCurrentToken(), 1);
	char* returnType = NULL;

	switch (t->kind)
	{
	case TOKEN_KW_RETURN:
	case TOKEN_ID:
	case TOKEN_OPEN_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENTS_SUFFIX-> STATEMENTS)\n");
		returnType = parse_STATEMENTS(outputFile);
		break;

	case TOKEN_KW_END:
	case TOKEN_CLOSE_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENTS_SUFFIX-> EPSILON)\n");
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"return id { end }", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return returnType;
}
char*			parse_STATEMENT				(FILE* outputFile)
{
	Token* t = peekN(getCurrentToken(), 1);
	char* returnType = NULL;

	switch (t->kind)
	{

	case TOKEN_OPEN_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENT ->  BLOCK)\n");
		returnType = parse_BLOCK(outputFile); // parse_BLOCK() will return the type of value that returned from the block if any.
		break;

	case TOKEN_KW_RETURN:
		fprintf(outputFile, "Rule(STATEMENT ->  return RETURN_SUFFIX)\n");
		match(TOKEN_KW_RETURN);
		returnType = parse_RETURN_SUFFIX(outputFile);
		break;

	case TOKEN_ID:
		fprintf(outputFile, "Rule(STATEMENT ->  id STATEMENT_SUFFIX)\n");
		match(TOKEN_ID);
		char* id_name = getIdLexeme();
		// TODO: check that the id already defined.
		parse_STATEMENT_SUFFIX(outputFile,id_name);
		// returnType is void.
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"{ return id", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return returnType;
}
void			parse_STATEMENT_SUFFIX		(FILE* outputFile, char* id_name)
{
	Token* t = peekN(getCurrentToken(), 1);
	IDInfoLinkNode* argumentsOfFunction = NULL;
	char* leftType = NULL;
	char* rightType = NULL;

	switch (t->kind)
	{
	case TOKEN_OPEN_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(STATEMENT_SUFFIX -> (PARAMETERS_LIST))\n");
		match(TOKEN_OPEN_ROUND_BRACKETS);
		argumentsOfFunction = parse_PARAMETERS_LIST(outputFile);
		match(TOKEN_CLOSE_ROUND_BRACKETS);
		checkFunctionArguments(id_name, argumentsOfFunction); // TODO: implement !!!
		break;

	case TOKEN_OPEN_SQUARE_BRACKETS:
	case TOKEN_ARITHMETIC_ASSIGNMENT:
		fprintf(outputFile, "Rule(STATEMENT_SUFFIX -> VARIABLE_SUFFIX = EXPRESSION)\n");
		leftType = parse_VARIABLE_SUFFIX(outputFile,id_name);
		match(TOKEN_ARITHMETIC_ASSIGNMENT);
		rightType = parse_EXPRESSION(outputFile);
		// TODO: type checking between left and right types.
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"( [ =", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
char* 			parse_RETURN_SUFFIX			(FILE* outputFile)
{
	//nullable - done
	Token* t = peekN(getCurrentToken(), 1);
	char* returnType = NULL;

	switch (t->kind)
	{
	case TOKEN_INT_NUMBER:
	case TOKEN_REAL_NUMBER:
	case TOKEN_ID:
		fprintf(outputFile, "Rule(RETURN_SUFFIX ->  EXPRESSION)\n");
		returnType = parse_EXPRESSION(outputFile);
		break;

	case TOKEN_SEMICOLON:
		fprintf(outputFile, "Rule(RETURN_SUFFIX -> EPSILON)\n");
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"int_number real_number id ;", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return returnType;
}
char*			parse_BLOCK					(FILE* outputFile)
{
	Token* t = peekN(getCurrentToken(), 1);
	char* returnedTypeOfBlock = NULL;

	switch (t->kind)
	{
	case TOKEN_OPEN_CURLY_BRACKETS:
		fprintf(outputFile, "Rule(BLOCK -> { VAR_DEFINITIONS; STATEMENTS })\n");
		match(TOKEN_OPEN_CURLY_BRACKETS);
		parse_BB();
		parse_VAR_DEFINITIONS(outputFile);
		match(TOKEN_SEMICOLON);
		returnedTypeOfBlock = parse_STATEMENTS(outputFile);
		if (returnedTypeOfBlock == NULL)
			returnedTypeOfBlock = strdup("void");
		parse_FB();
		match(TOKEN_CLOSE_CURLY_BRACKETS);
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"{", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	
	return returnedTypeOfBlock;
}
IDInfoLinkNode* parse_PARAMETERS_LIST		(FILE* outputFile)
{
	//nullable - done
	Token* t = peekN(getCurrentToken(), 1);
	IDInfoLinkNode* arguments = NULL;

	switch (t->kind)
	{
	case TOKEN_ID:
		fprintf(outputFile, "Rule(PARAMETERS_LIST ->  VARIABLES_LIST)\n");
		arguments = parse_VARIABLES_LIST(outputFile);
		break;

	case TOKEN_CLOSE_ROUND_BRACKETS:
		fprintf(outputFile, "Rule(PARAMETERS_LIST ->  EPSILON)\n");
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"id )", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
	return arguments;
}
char*			parse_EXPRESSION			(FILE* outputFile)
{
	Token* t = peekN(getCurrentToken(), 1);
	Token* tokenToCheck;
	char* returnType = NULL;

	switch (t->kind)
	{
	case TOKEN_INT_NUMBER:
		fprintf(outputFile, "Rule(EXPRESSION ->  int_number)\n");
		match(TOKEN_INT_NUMBER);
		returnType = strdup("integer");
		break;

	case TOKEN_REAL_NUMBER:
		fprintf(outputFile, "Rule(EXPRESSION ->  real_number)\n");
		match(TOKEN_REAL_NUMBER);
		returnType = strdup("real");
		break;

	case TOKEN_ID:
		tokenToCheck = peekN(t, 2); // looking to check what is the kind of the next token to decise how to act next.
		
		if (tokenToCheck->kind == TOKEN_ARITHMETIC_DIVISION || tokenToCheck->kind == TOKEN_ARITHMETIC_MULTIPLICATION) // checking that after id is 'ar_op'
		{
			fprintf(outputFile, "Rule(EXPRESSION ->  id ar_op EXPRESSION)\n");
			match(TOKEN_ID);
			char* id_name = getIdLexeme();
			ID_Information* IDInfo = find(id_name); // TODO: check that the id is already defined.
			next_token();	// skipping on token of kind: DIVISION/MULTIPLICATION - already checked in the if statement above.
			// TODO: if expression is int/real make sure to not divide by 0.
			char* expressinType = parse_EXPRESSION(outputFile);
			// TODO: decide what is the returnedType base on the guidelines.
		}
		else
		{
			fprintf(outputFile, "Rule(EXPRESSION ->  VARIABLE)\n");
			ID_Information* variableFromParse = parse_VARIABLE(outputFile);
			returnType = get_id_type(variableFromParse);
		}
		break;

	default:
		t = next_token();
		printf("Expected: one of tokens: %s at line %u,\nActual token : %s, lexeme: %s.\n",
			"int_number real_number id", t->lineNumber, tokenToString(t->kind), t->lexeme);
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
void			getAllFuctionDeclared(FILE* outputFile) {

	Token* t = getCurrentToken();

	// skipping forward untill "end" keyword:
	while (t->kind != TOKEN_KW_END)
		t = next_token();

	parse_FUNC_DEFINITIONS(outputFile);

	// skipping backward untill "program" keyword:
	while (t->kind != TOKEN_KW_PROGRAM)
		t = back_token();
}

void			parse_BB() {
	make_table();
}
void			parse_FB() {
	wereAllIDsUsed();
	pop_table();
}