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
#define EOFTOKEN	21
#define OTHER		22

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

// rows:     current state mod 100
// columns: input character class
int delta[][] = {
	/*	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22*/	
	/*  0 */ {}
	/*  1 */
	/*  2 */
	/*  3 */
	/*  4 */
	/*  5 */
	/*  6 */
	/*  7 */
	/*  8 */
	/*  9 */
	/* 10 */
	/* 11 */
	/* 12 */
	/* 13 */
	/* 14 */
	/* 15 */
	/* 16 */
	/* 17 */
	/* 18 */
	/* 19 */
	/* 20 */
	/* 21 */
	/* 22 */
	/* 23 */
	/* 24 */
	/* 25 */
	/* 26 */
	/* 27 */
	/* 28 */
	/* 29 */
	/* 30 */
	/* 31 */
	/* 32 */
	/* 33 */
	/* 34 */
	/* 35 */
	/* 36 */
	/* 37 */
	/* 38 */
	/* 39 */
	/* 40 */
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
