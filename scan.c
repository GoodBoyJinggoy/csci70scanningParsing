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
// #define LETTER		3 // list, all letters 
#define LETTER_NOEXP	4 // list, all letters except e,E
#define LETTER_EXP	5 // list, e,E
#define DIGIT		6 // list, 0-9
#define UNDERSCORE	7
#define QUOTMARK	8
#define COLON		9
#define SEMICOLON	10
#define GT		11
#define LT		12
#define EQUAL		13
#define COMMA		14
#define LPAREN		15
#define RPAREN		16
#define PLUS		17
#define MINUS		18
#define ASTERISK	19
#define SLASH		20
#define EXCLAMATION	21
#define EOFTOKEN	22
#define OTHER		24

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

int delta[][] = {
	
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
