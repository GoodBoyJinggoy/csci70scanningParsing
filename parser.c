#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include"scan.h"
#include"parser.h"

struct node* tempStorage; 
struct node* inp;

void parseerror(char *message){
	printf("%s\n", message);
	exit(1);
}

void consume(struct node **inpPtr){
	*inpPtr = (*inpPtr) -> next; 
}

void match(struct node **inpPtr, int expected){
	if(((*inpPtr) -> value).id == expected){
		consume(inpPtr);
	}
	else{
		char *expectedTokenName;
		strcpy(expectedTokenName,tokennames[expected]);
		char *msg = " expected";
		parseerror(strcat(expectedTokenName,msg));
	}
}

void prg(struct node **inpPtr){
	blk(inpPtr);
	match(inpPtr, TokenEOF);
}

void blk(struct node **inpPtr){
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
	switch( ((*inpPtr) -> value).id ){
		case TokenIdentifier:
			match(inpPtr, TokenIdentifier);
			match(inpPtr, TokenAssign);
			exp(inpPtr);
			match(inpPtr, TokenSemicolon);
			printf("Assignment Statement Recognized\n");
			break;
		case TokenPRINT:
			match(inpPtr, TokenPRINT);
			//debug
			match(inpPtr, TokenLeftParen);
			arg(inpPtr);
			argfollow(inpPtr);
			match(inpPtr, TokenRightParen);
			match(inpPtr, TokenSemicolon);
			printf("Print Statement Recognized\n");
			break;
		case TokenIF:
			match(inpPtr, TokenIF);
			printf("If Statement Begins\n");
			cnd(inpPtr);
			match(inpPtr, TokenColon);
			blk(inpPtr);
			iffollow(inpPtr);
			printf("If Statement Ends\n");
			break;
		default: 
			parseerror("Invalid Statement\n");
			break;
	}
}

void argfollow(struct node **inpPtr){
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
			parseerror("Incomplete if Statement\n");
			break;
	}
}

void exp(struct node **inpPtr){
	trm(inpPtr);
	trmfollow(inpPtr);
}

void trmfollow(struct node **inpPtr){
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
	fac(inpPtr);
	facfollow(inpPtr);
}

void facfollow(struct node **inpPtr){
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
	lit(inpPtr);
	litfollow(inpPtr);
}

void litfollow(struct node **inpPtr){
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
	exp(inpPtr);
	rel(inpPtr);
	exp(inpPtr);
}

void rel(struct node **inpPtr){
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
			parseerror("Missing relational operator\n");
			break;
	}
}

int main(int argc, char** argv){
	tempStorage = (struct node*) malloc(2 * sizeof(struct node));
	int i = 1;
	char filename[50];
	if(argc >= 2){
		strcpy(filename,argv[1]);
	}
	openfile(filename);
	struct token t = gettoken(false);
	// head
	tempStorage[0].value = t;
	struct node *curr = &tempStorage[0];
	while(t.id != TokenEOF){
		t = gettoken(false);
		tempStorage[i].value = t;
		curr -> next = &tempStorage[i];
		curr = &tempStorage[i];
		i++;
		tempStorage = (struct node*) realloc(tempStorage,(i + 1) * sizeof(struct node));
	}
	inp = &tempStorage[0];
	prg(&inp);
	printf("%s is a valid SimpCalc program\n", filename);
	free(tempStorage);

}
