%{
#include "Utils.h"
#include "Token.h"

int lineNumber = 1;
%}

DIGIT		[0-9]
NONZERO		[1-9]
ALPHA		[a-zA-Z]
ALPHANUM	[a-zA-Z0-9]

ID				{ALPHA}("_"?{ALPHANUM})*
NUMBER_INT		({NONZERO}{DIGIT}*|0)
NUMBER_REAL	    ({NONZERO}{DIGIT}*|0)"."{DIGIT}+
COMMENT			"--".*
ENDLN			[\n]
WHITE			[ \t]


%%

"program"											{return handleValidToken(yyout, TOKEN_KW_PROGRAM,				yytext, lineNumber);}
"end"												{return handleValidToken(yyout, TOKEN_KW_END,					yytext, lineNumber);}
"real"												{return handleValidToken(yyout, TOKEN_KW_REAL,					yytext, lineNumber);}
"integer"											{return handleValidToken(yyout, TOKEN_KW_INTEGER,				yytext, lineNumber);}
"void"												{return handleValidToken(yyout, TOKEN_KW_VOID,					yytext, lineNumber);}
"return"											{return handleValidToken(yyout, TOKEN_KW_RETURN,				yytext, lineNumber);}

"/"													{return handleValidToken(yyout, TOKEN_ARITHMETIC_DIVISION,		yytext, lineNumber);}
"*"													{return handleValidToken(yyout, TOKEN_ARITHMETIC_MULTIPLICATION,yytext, lineNumber);}
"="													{return handleValidToken(yyout, TOKEN_ARITHMETIC_ASSIGNMENT,	yytext, lineNumber);}

";"													{return handleValidToken(yyout, TOKEN_SEMICOLON,				yytext, lineNumber);}
","													{return handleValidToken(yyout, TOKEN_COMMA,					yytext, lineNumber);}

"{"													{return handleValidToken(yyout, TOKEN_OPEN_CURLY_BRACKETS,		yytext, lineNumber);}
"}"													{return handleValidToken(yyout, TOKEN_CLOSE_CURLY_BRACKETS,		yytext, lineNumber);}

"["													{return handleValidToken(yyout, TOKEN_OPEN_SQUARE_BRACKETS,		yytext, lineNumber);}
"]"													{return handleValidToken(yyout, TOKEN_CLOSE_SQUARE_BRACKETS,	yytext, lineNumber);}

"("													{return handleValidToken(yyout, TOKEN_OPEN_ROUND_BRACKETS,		yytext, lineNumber);}
")"													{return handleValidToken(yyout, TOKEN_CLOSE_ROUND_BRACKETS,		yytext, lineNumber);}


{ID}												{return handleValidToken(yyout, TOKEN_ID,						yytext, lineNumber);}
{NUMBER_INT}										{return handleValidToken(yyout, TOKEN_INT_NUMBER,				yytext, lineNumber);}
{NUMBER_REAL}										{return handleValidToken(yyout, TOKEN_REAL_NUMBER,				yytext, lineNumber);}

{ENDLN}												lineNumber++;
{COMMENT}											{}
{WHITE}												{}
<<EOF>>												{return handleValidToken(yyout, TOKEN_END_OF_FILE,				yytext, lineNumber);}

.													printInvalidTokenToConsole(yytext, lineNumber);


%%



int main(int argc, char** argv) 
{
	parseInputFile("C:\\temp\\test1.txt", 
			 "c:\\temp\\test1_203605654_313583205_lex.txt",
			 "c:\\temp\\test1_203605654_313583205_syntactic.txt");

	lineNumber = 1;

	parseInputFile("C:\\temp\\test2.txt", 
			 "c:\\temp\\test2_203605654_313583205_lex.txt",
			 "c:\\temp\\test2_203605654_313583205_syntactic.txt");
	
}
