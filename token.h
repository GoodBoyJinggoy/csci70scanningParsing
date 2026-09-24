struct token
{
	int id;
	char lexeme[50];
};

extern const char *tokennames[];

#define TokenIdentifier 1
#define TokenNumber 	2
#define TokenString 	3
#define TokenAssign 	4
#define TokenSemicolon	5
#define TokenColon	6
#define TokenComma	7
#define TokenLeftParen	8
#define TokenRightParen	9
#define TokenPlus	10
#define TokenMinus	11
#define TokenMultiply	12
#define TokenDivide	13
#define TokenRaise 	14
#define TokenLT		15
#define TokenEqual	16
#define TokenGT		17
#define TokenLTE	18
#define TokenGTE	19
#define TokenNotEqual	20
#define TokenEOF	21
