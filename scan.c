#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include<scan.h>

#define MAXLINELEN = 1000;

// character classes, lists
#define NEWLINE 	0 
#define SPACE 		1
#define TAB		2
#define LETTER_NOEXP	3 // list, all letters except e,E
#define LETTER_EXP	4 // list, e,E
#define DIGIT		5 // list, 0-9
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

/*
const char letters[] = {
	'a','A','b','B','c','C','d','D','e','E','f','F',
	'g','G','h','H','i','I','j','J','k','K','l','L',
	'm','M','n','N','o','O','p','P','q','Q','r','R',
	's','S','t','T','u','U','v','V','w','W','x','X',
	'y','Y','z','Z'
}; 
*/

const char letters_exp[] = {'e','E'};
const char letters_noexp[] = {
	'a','A','b','B','c','C','d','D','f','F',
	'g','G','h','H','i','I','j','J','k','K','l','L',
	'm','M','n','N','o','O','p','P','q','Q','r','R',
	's','S','t','T','u','U','v','V','w','W','x','X',
	'y','Y','z','Z'
}; 

const char digits[] = {'0','1','2','3','4','5','6','7','8','9'};

bool inArr(char c, char arr[]){
	size_t len = sizeof(arr)/sizeof(arr[0]);
	for(int i = 0; i < len; i++){
		if(c == arr[i]){
			return true;
		}
	}
	return false;
}

FILE *input;
static int linenum = 1;
char line[MAXLINELEN];
int len = 0;
int ptr = 1;
bool pushback = false;
char charread = '\0';

// inputs:
// rows:    current state mod 100
// columns: character class
int delta[][] = {
	/*	     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23*/	
	/*  0 */ {   0,   0,   0,  33,  33,  34,  33,  16, 32,  105,  31,  30, 116, 107, 108, 109, 110, 111,  26,  27,   5, 322, 121, 322},
	/*  1 */ {}, 
	/*  2 */ {}, 
	/*  3 */ {}, 
	/*  4 */ {}, 
	/*  5 */ {}, 
	/*  6 */ {}, 
	/*  7 */ {}, 
	/*  8 */ {}, 
	/*  9 */ {}, 
	/* 10 */ {}, 
	/* 11 */ {}, 
	/* 12 */ {}, 
	/* 13 */ {}, 
	/* 14 */ {}, 
	/* 15 */ {}, 
	/* 16 */ {}, 
	/* 17 */ {}, 
	/* 18 */ {}, 
	/* 19 */ {}, 
	/* 20 */ {}, 
	/* 21 */ {}, 
	/* 22 */ {}, 
	/* 23 */ {}, 
	/* 24 */ {}, 
	/* 25 */ {}, 
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

int charclass(char c){
	if(inArr(c, digits)){
		return DIGIT;
	}
	
	if(inArr(c, letters_exp){
		return LETTER_EXP;
	}
	if(inArr(c, letters_noexp){
		return LETTER_NOEXP;
	}
	switch(c){
		case '\n': return NEWLINE;
		case  ' ': return SPACE;
		case '\t': return TAB;
		case  '_': return UNDERSCORE;
		case  '"': return QUOTMARK;
		case  ':': return COLON;
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
		case  EOF: return EOFTOKEN;
		default  : return OTHER;	
	}
}

int openfile(char *filename)
{
	input = fopen(filename, "r");
	if(input == NULL)
	{
		printf("File not found.");
		exit(1);
	}
	return 0;
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

struct token gettoken()
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
		if(state == 0){
			strcpy(temp.lexeme, "");
		}
		else if(state/100 == 0){
			strcat(temp.lexeme, placeholder);
		}
	}
	if(state/100 == 3){ 
		printf("Error lol");
		temp.id = 0; // error id
		strcpy(temp.lexeme,"");
	}
	else if(state/100 == 2){
		temp.id = state % 100;
		pushback = true;
	}
	else if(state/100 == 1){
		temp.id = state % 100;
		pushback = false;
	}
	return temp;
}
