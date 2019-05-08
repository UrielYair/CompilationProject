%{
#include "Utils.h"
#include "Token.h"
%}

DIGIT			[0-9]
ALPHA			[a-zA-Z]
NATURAL_NUMBER	[1-9]{DIGIT}*
ID				{ALPHA}("_"?{ALPHA}|{DIGIT})*


%%

--.*												{}
\n													lineNumber++;
<<EOF>>												{handleValidToken(yyout, TOKEN_END_OF_FILE,				yytext, lineNumber);
													return 0;}

(" ")+												{}

"/"													handleValidToken(yyout, TOKEN_ARITHMETIC_DIVISION,		yytext, lineNumber);
"*"													handleValidToken(yyout, TOKEN_ARITHMETIC_MULTIPLICATION,yytext, lineNumber);
"="													handleValidToken(yyout, TOKEN_OP_EQUAL,					yytext, lineNumber);

","													handleValidToken(yyout, TOKEN_COMMA,					yytext, lineNumber);
";"													handleValidToken(yyout, TOKEN_SEMICOLON,				yytext, lineNumber);

"{"													handleValidToken(yyout, TOKEN_OPEN_CURLY_BRACKETS,		yytext, lineNumber);
"}"													handleValidToken(yyout, TOKEN_CLOSE_CURLY_BRACKETS,		yytext, lineNumber);

"["													handleValidToken(yyout, TOKEN_OPEN_SQUARE_BRACKETS,		yytext, lineNumber);
"]"													handleValidToken(yyout, TOKEN_CLOSE_SQUARE_BRACKETS,	yytext, lineNumber);

"("													handleValidToken(yyout, TOKEN_OPEN_ROUND_BRACKETS,		yytext, lineNumber);
")"													handleValidToken(yyout, TOKEN_CLOSE_ROUND_BRACKETS,		yytext, lineNumber);


"program"											handleValidToken(yyout, TOKEN_KW_PROGRAM,				yytext, lineNumber);
"end"												handleValidToken(yyout, TOKEN_KW_END,					yytext, lineNumber);
"void"												handleValidToken(yyout, TOKEN_KW_VOID,					yytext, lineNumber);
"return"											handleValidToken(yyout, TOKEN_KW_RETURN,				yytext, lineNumber);
"integer"											handleValidToken(yyout, TOKEN_KW_INTEGER,				yytext, lineNumber);
"real"												handleValidToken(yyout, TOKEN_KW_REAL,					yytext, lineNumber);


0|{NATURAL_NUMBER}									handleValidToken(yyout, TOKEN_INT_NUMBER,				yytext, lineNumber);
0"."{DIGIT}+|{NATURAL_NUMBER}"."{DIGIT}+			handleValidToken(yyout, TOKEN_REAL_NUMBER,				yytext, lineNumber);

"."{DIGIT}+|"."{NATURAL_NUMBER}"."{DIGIT}+			printInvalidTokenToConsole(yytext, lineNumber);
0"."|{NATURAL_NUMBER}"."							printInvalidTokenToConsole(yytext, lineNumber);



" "*[^_]"_"{ID}|{ID}"_"[^_]" "?						printInvalidTokenToConsole(yytext, lineNumber);

{ID}												handleValidToken(yyout, TOKEN_ID,	yytext, lineNumber);

.													printInvalidTokenToConsole(yytext, lineNumber);



%%

int main(int argc, char** argv) 
{

	int lineNumber=1;
	lexCheck("C:\\temp\\test1.txt", 
			 "c:\\temp\\test1_203605654_302846621_lex.txt");
	
	// method to get first node and assign it to *currentNode.
	// int currentIndex = 0;
	
	// parse_PROGRAM();
	// match(TOKEN_END_OF_FILE);
	//

	//test1
	//free nodes 



	//=======================================================

	
	lineNumber=1;
	lexCheck("C:\\temp\\test2.txt", 
			 "c:\\temp\\test2_203605654_302846621_lex.txt");


	// parse_PROGRAM();
	// match(TOKEN_END_OF_FILE);
	//

	// method to get first node and assign it to *currentNode.
	// int currentIndex = 0;

	//test 2
	//free nodes 
}
