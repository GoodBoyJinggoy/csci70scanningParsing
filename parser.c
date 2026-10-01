#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include"scan.h"

#define MAXTOKENS 2000
int main(int argc, char** argv){
	struct node tempStorage[MAXTOKENS];
	int i = 0;
	char filename[50];
	if(argc >= 2){
		strcpy(filename,argv[1]);
	}
	openfile(filename);
	struct token t = gettoken();
	struct node head;
	head.value = t;
	struct node *curr = &head;
	while(t.id != TokenEOF){
		t = gettoken();
		tempStorage[i].value = t;
		curr -> next = &tempStorage[i];
		curr = &tempStorage[i];
		i++;
	}
	struct node *iter = &head;
	while(iter -> next != NULL){
		printf("id: %d, lexeme: %s\n", iter -> value.id, iter -> value.lexeme);
		iter = iter -> next;
	}
}
