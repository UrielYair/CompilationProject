#include "Token.h"

/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

int currentIndex = 0;
Node* currentNode = NULL;

#define TOKEN_ARRAY_SIZE 100

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{ 
	int length = strlen(lexeme)+1;
	
	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if(currentNode == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);
		if(currentNode->tokensArray == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if(currentNode == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);

			if(currentNode->tokensArray == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;	
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*length);
	#ifdef _WIN32
		strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
	#else
		strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	#endif		
}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token *back_token() {  
	if(currentIndex == 0) 
	{
		// Previous node is needed.
		if (currentNode->prev == NULL) 
		{
			// Previous node is not exist (the program is on the first node which created).
			printf("There is no previous node in the memory.\n");
			return NULL;
		}
		else
		{
			// Retrieving previous node.
			currentNode = currentNode->prev;
			currentIndex = 99;
			return &(currentNode->tokensArray)[currentIndex];
		}
	}
	else
	{
		// Previous node is not needed.
		// Update currentIndex only and return.
		currentIndex--;
		return &(currentNode->tokensArray)[currentIndex];
	}
	
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token): 
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token* next_token() 
{
	if (currentIndex == 99)
	{
		// Next node is needed.
		if (currentNode->next == NULL)
		{
			// The program is currently on the last token that stores in the memory.
			printf("No further tokens in the memory, The program is currently on the last one.\n");
			return NULL;
		}
		else
		{
			// Retrieving next node.
			currentNode = currentNode->next;
			currentIndex = 0;
			return &(currentNode->tokensArray)[currentIndex];
		}
	}
	else
	{
		// Next node is not needed.
		currentIndex++;
		if (&currentNode->tokensArray[currentIndex] == NULL)
		{
			// The program is currently on the last token that stores in the memory.
			printf("No further tokens in the memory, The program is currently on the last one.\n");
			return NULL;
		}
		else
			return &(currentNode->tokensArray)[currentIndex];
	}

}

int match(eTOKENS expectedToken) //TODO: check how to use bool.
{

	Token* currentToken = next_token();

	// TODO: fix messeges.
	if (expectedToken != currentToken->kind)
	{
		printf("Hatul Hatul Hatul Hatul Hatul Hatul !!!! .\n");
		return 0;  // false
	}
	else
	{
		printf("Hatul Hatul Hatul Hatul Hatul Hatul !!!! .\n");
		return 1;  // true
	}
}

Token* peekN(Token* current, int numberOfNext)
{
	for (int i = 0; i < numberOfNext; i++)
	{
		next_token();
	}

	Token* returnToken = &currentNode->tokensArray[currentIndex];
	
	for (int i = 0; i < numberOfNext; i++)
	{
		back_token();
	}
	return returnToken;
}



const char* tokenToString(enum eTOKENS kind)
{
	switch (kind)
	{
		case TOKEN_ID:						return "TOKEN_ID";
		case TOKEN_KW_INTEGER:				return "TOKEN_KW_INTEGER";
		case TOKEN_KW_REAL:					return "TOKEN_KW_REAL";
		case TOKEN_KW_PROGRAM:				return "TOKEN_KW_PROGRAM";
		case TOKEN_KW_END:					return "TOKEN_KW_END";
		case TOKEN_KW_VOID:					return "TOKEN_KW_VOID";
		case TOKEN_KW_RETURN:				return "TOKEN_KW_RETURN";
		case TOKEN_END_OF_FILE:				return "TOKEN_END_OF_FILE";
		case TOKEN_COMMA:					return "TOKEN_COMMA";
		case TOKEN_SEMICOLON:				return "TOKEN_SEMICOLON";
		case TOKEN_OPEN_CURLY_BRACKETS:		return "TOKEN_OPEN_CURLY_BRACKETS";
		case TOKEN_CLOSE_CURLY_BRACKETS:	return "TOKEN_CLOSE_CURLY_BRACKETS";
		case TOKEN_OPEN_SQUARE_BRACKETS:	return "TOKEN_OPEN_SQUARE_BRACKETS";
		case TOKEN_CLOSE_SQUARE_BRACKETS:	return "TOKEN_CLOSE_SQUARE_BRACKETS";
		case TOKEN_OPEN_ROUND_BRACKETS:		return "TOKEN_OPEN_ROUND_BRACKETS";
		case TOKEN_CLOSE_ROUND_BRACKETS:	return "TOKEN_CLOSE_ROUND_BRACKETS";
		case TOKEN_ARITHMETIC_MULTIPLICATION: return "TOKEN_ARITHMETIC_MULTIPLICATION";
		case TOKEN_ARITHMETIC_DIVISION:		return "TOKEN_ARITHMETIC_DIVISION";
		case TOKEN_OP_EQUAL:				return "TOKEN_OP_EQUAL";
		case TOKEN_INT_NUMBER:				return "TOKEN_INT_NUMBER";
		case TOKEN_REAL_NUMBER:				return "TOKEN_REAL_NUMBER";
		default:							return "enum not valid.";
	}
}

void handleValidToken(FILE* outputFile, eTOKENS kind, char* lexeme, int numOfLine) 
{
	create_and_store_token(kind, lexeme, numOfLine);
	printValidTokenToOutputFile(yyout, kind, lexeme, numOfLine);
}
