#include "token.h"
#include <stdbool.h>

// linked list node containing tokens, for use in parser
// line number included for printing errors in parser
struct node{
	struct token value;
	int linenum;
	struct node *next;
};

bool isSuccessful();
void confirmsuccess(char *filename);
int openfile(char *inputFilename, char *outputFilename);
void closefiles();
void printtoken(struct token t);
struct token gettoken(bool isScanner);
int getlinenumber();
void resetlinenumber();
void parseerror(int linenum, char *message);
void consume(struct node **inpPtr);
void match(struct node **inpPtr, int expected);
void prg(struct node **inpPtr);
void blk(struct node **inpPtr);
void stm(struct node **inpPtr);
void argfollow(struct node **inpPtr);
void arg(struct node **inpPtr);
void iffollow(struct node **inpPtr);
void exp(struct node **inpPtr);
void trmfollow(struct node **inpPtr);
void trm(struct node **inpPtr);
void facfollow(struct node **inpPtr);
void fac(struct node **inpPtr);
void litfollow(struct node **inpPtr);
void lit(struct node **inpPtr);
void val(struct node **inpPtr);
void cnd(struct node **inpPtr);
void rel(struct node **inpPtr);

