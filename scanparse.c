#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include"scanparse.h"

#define MAXLINELEN 2000
#define LETTER_EXP_SIZE 2
#define LETTER_NOEXP_SIZE 50
#define DIGIT_SIZE 10
// character classes
#define NEWLINE 	0 
#define SPACE 		1
#define TAB		2
#define LETTER_NOEXP	3 // all letters except e,E
#define LETTER_EXP	4 // e,E
#define DIGIT		5 // 0-9
#define UNDERSCORE	6
#define QUOTMARK	7	
#define COLON		8	
#define SEMICOLON	9	
#define GT		10
#define LT		11
#define EQUAL		12
#define COMMA		13
#define LPAREN		14
#define RPAREN		15
#define PLUS		16
#define MINUS		17
#define ASTERISK	18
#define SLASH		19
#define EXCLAMATION	20
#define PERIOD		21
#define EOFTOKEN	22
#define OTHER		23

const char letters_exp[] = {'e','E'};
const char letters_noexp[] = {
	'a','A','b','B','c','C','d','D','f','F',
	'g','G','h','H','i','I','j','J','k','K','l','L',
	'm','M','n','N','o','O','p','P','q','Q','r','R',
	's','S','t','T','u','U','v','V','w','W','x','X',
	'y','Y','z','Z'
}; 

const char digits[] = {'0','1','2','3','4','5','6','7','8','9'};

bool inArr(char c, const char arr[], int len){
	for(int i = 0; i < len; i++){
		if(c == arr[i]){
			return true;
		}
	}
	return false;
}

static int linenum = 1;
FILE *input;
FILE *output;
char line[MAXLINELEN];
int len = 0;
int ptr = 1;
bool pushback = false;
char charread = '\0';

// scanner logic

// state table
// rows:    current state mod 100
// columns: character class (input)
int delta[][24] = {
	/*	     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23*/	
	/*  0 */ {   0,   0,   0,  33,  33,  34,  33,  40, 32,  105,  31,  30, 116, 107, 108, 109, 110, 111,  26,  27,  29, 322, 121, 322},
	/*  1 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/*  2 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/*  3 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/*  4 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/*  5 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/*  6 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/*  7 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/*  8 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/*  9 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 10 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 11 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 12 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 13 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 14 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 15 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 16 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 17 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 18 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 19 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 20 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 21 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 22 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 23 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 24 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 25 */ {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
	/* 26 */ { 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 114, 212, 212, 212, 212, 212}, 
	/* 27 */ { 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213,  28, 213, 213, 213, 213},
	/* 28 */ {   0,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28},
	/* 29 */ { 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 120, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323},
	/* 30 */ { 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 118, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215},
	/* 31 */ { 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 119, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217},
	/* 32 */ { 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 104, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206},
	/* 33 */ { 201, 201, 201,  33,  33,  33,  33, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201},
	/* 34 */ { 202, 202, 202, 202,  37,  34, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202,  35, 202, 202},
	/* 35 */ { 325, 325, 325, 325, 325,  36, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325}, 
	/* 36 */ { 202, 202, 202, 202,  37,  36, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202}, 
	/* 37 */ { 325, 325, 325, 325, 325,  39, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325,  38,  38, 325, 325, 325, 325, 325, 325}, 
	/* 38 */ { 325, 325, 325, 325, 325,  39, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325},
	/* 39 */ { 202, 202, 202, 202, 202,  39, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202},
	/* 40 */ { 324,  40,  40,  40,  40,  40,  40, 103,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40},
};

// categorize character
int charclass(char c){
	if(inArr(c, digits, DIGIT_SIZE)){
		return DIGIT;
	}
	if(inArr(c, letters_exp, LETTER_EXP_SIZE)){
		return LETTER_EXP;
	}
	if(inArr(c, letters_noexp, LETTER_NOEXP_SIZE)){
		return LETTER_NOEXP;
	}
	switch(c){
		case '\r':
		case '\n': return NEWLINE;
		case  ' ': return SPACE;
		case '\t': return TAB;
		case  '_': return UNDERSCORE;
		case '\"': return QUOTMARK;
		case  ':': return COLON;
		case  ';': return SEMICOLON;
		case  '>': return GT;
		case  '<': return LT;
		case  '=': return EQUAL;
		case  ',': return COMMA;
		case  '(': return LPAREN;
		case  ')': return RPAREN;
		case  '+': return PLUS;
		case  '-': return MINUS;
		case  '*': return ASTERISK;
		case  '/': return SLASH;
		case  '!': return EXCLAMATION;
		case  '.': return PERIOD;
		case  EOF: return EOFTOKEN;
		default  : return OTHER;	
	}
}

// print token ID and lexeme to output file
void printtoken(struct token t){
	fprintf(output, "%s %s\n", tokennames[t.id], t.lexeme);
}

// open input and output files
int openfile(char *inputFilename, char *outputFilename)
{
	input = fopen(inputFilename, "r");
	if(input == NULL)
	{
		printf("File not found.");
		exit(1);
	}
	output = fopen(outputFilename, "w");
	return 0;
}

// close input and output files
void closefiles(){
	fclose(input);
	fclose(output);
}

void resetlinenumber(){
	linenum = 1;
}
int getlinenumber(){
	return linenum;
}

char mygetchar()
{
	if(pushback)
	{
		pushback = false;
	}
	else
	{
		charread = fgetc(input);
		if(charread == '\n'){
			linenum++;
		}
	}
	return charread;
}

const char *errormessage(int errnum)
{
	switch(errnum)
	{
		case 322: return "Illegal character/character sequence";
		case 323: return "Illegal character/character sequence";
		case 324: return "Unterminated string";
		case 325: return "Invalid number format"; 
		default:  return "Unspecified error";
	}
}

struct token checkSpecialIdent(struct token t)
{
	char *lexeme = t.lexeme;
	if(strcmp(lexeme,"PRINT\0") == 0){
		t.id = TokenPRINT;
	}
	else if(strcmp(lexeme, "IF\0") == 0){
		t.id = TokenIF;
	}
	else if(strcmp(lexeme, "ELSE\0") == 0){
		t.id = TokenELSE;
	}
	else if(strcmp(lexeme, "ENDIF\0") == 0){
		t.id = TokenENDIF;
	}
	else if(strcmp(lexeme, "SQRT\0") == 0){
		t.id = TokenSQRT;
	}
	else if(strcmp(lexeme, "AND\0") == 0){
		t.id = TokenAND;
	}
	else if(strcmp(lexeme, "OR\0") == 0){
		t.id = TokenOR;
	}
	else if(strcmp(lexeme, "NOT\0") == 0){
		t.id = TokenNOT;
	}
	return t;
}

struct token gettoken(bool isScanner)
{
	int state = 0;
	struct token temp;
	strcpy(temp.lexeme,"");
	char placeholder[] = {'.','\0'};
	while(state == 0 || (state >= 26 && state < 99))
	{
		char c = mygetchar();
		placeholder[0] = c;
		int ch = charclass(c);
		state = delta[state][ch];
		if(state == 0 || state == 28){
			strcpy(temp.lexeme, "");
		}
		else if(state/100 == 0){
			strcat(temp.lexeme, placeholder);
		}
	}
	if(state/100 == 3){ 
		if(isScanner){
			fprintf(output, "Lexical Error: %s (line #%i)\n", errormessage(state), linenum);
		}
		temp.id = 0; // error id
		strcpy(temp.lexeme,"");
	}
	else if(state/100 == 2){
		temp.id = state % 100;
		pushback = true;
		// special identifiers
		if(state == 201){
			temp = checkSpecialIdent(temp);
		}
	}
	else if(state/100 == 1){
		strcat(temp.lexeme, placeholder);
		temp.id = state % 100;
		pushback = false;
	}
	return temp;
}

// parser logic

// boolean for checking error conditions in the parser
bool parseSuccess;

bool isSuccessful(){
	return parseSuccess;
}

void confirmsuccess(char *filename){
	fprintf(output, "%s is a valid SimpCalc program\n", filename);
}

void parseerror(int linenum, char *message){
	fprintf(output,"Parse Error: %s. (line #%d)\n", message, linenum);
	parseSuccess = false;
}

void consume(struct node **inpPtr){
	*inpPtr = (*inpPtr) -> next; 
}

void match(struct node **inpPtr, int expected){
	if(parseSuccess == false) { return; }
	if(((*inpPtr) -> value).id == expected){
		consume(inpPtr);
	}
	else{
		char expectedTokenName[30];
		strcpy(expectedTokenName,tokennames[expected]);
		char msg[] = " expected";
		parseerror((*inpPtr) -> linenum, strcat(expectedTokenName,msg));
	}
}

// start of recursive descent parser
void prg(struct node **inpPtr){
	parseSuccess = true;
	blk(inpPtr);
	match(inpPtr, TokenEOF);
}

void blk(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenIdentifier:
		case TokenPRINT:
		case TokenIF:
			stm(inpPtr);
			blk(inpPtr);
			break;
		default:
			break;
	}
}

void stm(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenIdentifier:
			match(inpPtr, TokenIdentifier);
			match(inpPtr, TokenAssign);
			exp(inpPtr);
			match(inpPtr, TokenSemicolon);
			if(parseSuccess == true){ 
				fprintf(output, "Assignment Statement Recognized\n");
			}
			break;
		case TokenPRINT:
			match(inpPtr, TokenPRINT);
			match(inpPtr, TokenLeftParen);
			arg(inpPtr);
			argfollow(inpPtr);
			match(inpPtr, TokenRightParen);
			match(inpPtr, TokenSemicolon);
			if(parseSuccess == true){
				fprintf(output, "Print Statement Recognized\n");
			}
			break;
		case TokenIF:
			match(inpPtr, TokenIF);
			if(parseSuccess == true){
				fprintf(output, "If Statement Begins\n");
			};
			cnd(inpPtr);
			match(inpPtr, TokenColon);
			blk(inpPtr);
			iffollow(inpPtr);
			if(parseSuccess == true){
				fprintf(output, "If Statement Ends\n");
			}
			break;
		default: 
			parseerror((*inpPtr) -> linenum, "Invalid Statement");
			break;
	}
}

void argfollow(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenComma:
			match(inpPtr, TokenComma);
			arg(inpPtr);
			argfollow(inpPtr);
			break;
		default:
			break;
	}
}

void arg(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenString:
			match(inpPtr, TokenString);
			break;
		default:
			exp(inpPtr);
			break;
	}
}

void iffollow(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenENDIF:
			match(inpPtr, TokenENDIF);
			match(inpPtr, TokenSemicolon);
			break;
		case TokenELSE:
			match(inpPtr, TokenELSE);
			blk(inpPtr);
			match(inpPtr, TokenENDIF);
			match(inpPtr, TokenSemicolon);
			break;
		default:
			parseerror((*inpPtr) -> linenum, "Incomplete if Statement");
			break;
	}
}

void exp(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	trm(inpPtr);
	trmfollow(inpPtr);
}

void trmfollow(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenPlus:
			match(inpPtr, TokenPlus);
			trm(inpPtr);
			trmfollow(inpPtr);
			break;
		case TokenMinus:
			match(inpPtr, TokenMinus);
			trm(inpPtr);
			trmfollow(inpPtr);
			break;
		default:
			break;
	}
}

void trm(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	fac(inpPtr);
	facfollow(inpPtr);
}

void facfollow(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenMultiply:
			match(inpPtr, TokenMultiply);
			fac(inpPtr);
			facfollow(inpPtr);
			break;
		case TokenDivide:
			match(inpPtr, TokenDivide);
			fac(inpPtr);
			facfollow(inpPtr);
			break;
		default:
			break;
	}
}

void fac(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	lit(inpPtr);
	litfollow(inpPtr);
}

void litfollow(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenRaise:
			match(inpPtr, TokenRaise);
			lit(inpPtr);
			litfollow(inpPtr);
			break;
		default:
			break;
	}
}

void lit(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenMinus:
			match(inpPtr, TokenMinus);
			val(inpPtr);
			break;
		default:
			val(inpPtr);
			break;
	}
}

void val(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenIdentifier:
			match(inpPtr, TokenIdentifier);
			break;
		case TokenNumber:
			match(inpPtr, TokenNumber);
			break;
		case TokenSQRT:
			match(inpPtr, TokenSQRT);
			match(inpPtr, TokenLeftParen);
			exp(inpPtr);
			match(inpPtr, TokenRightParen);
			break;
		default:
			match(inpPtr, TokenLeftParen);
			exp(inpPtr);
			match(inpPtr, TokenRightParen);
			break;
	}
}

void cnd(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	exp(inpPtr);
	rel(inpPtr);
	exp(inpPtr);
}

void rel(struct node **inpPtr){
	if(parseSuccess == false) { return; }
	switch( ((*inpPtr) -> value).id ){
		case TokenLT:
			match(inpPtr, TokenLT);
			break;
		case TokenEqual:
			match(inpPtr, TokenEqual);
			break;
		case TokenGT:
			match(inpPtr, TokenGT);
			break;
		case TokenGTE:
			match(inpPtr, TokenGTE);
			break;
		case TokenNotEqual:
			match(inpPtr, TokenNotEqual);
			break;
		case TokenLTE:
			match(inpPtr, TokenLTE);
			break;
		default:
			parseerror((*inpPtr) -> linenum, "Missing relational operator");
			break;
	}
}
