#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include"scan.h"

int main(int argc, char** argv){
	struct node* tempStorage = (struct node*) malloc(2 * sizeof(struct node));
	int i = 1;
	char filename[50];
	if(argc >= 2){
		strcpy(filename,argv[1]);
	}
	openfile(filename);
	struct token t = gettoken();
	// head
	tempStorage[0].value = t;
	struct node *curr = &tempStorage[0];
	while(t.id != TokenEOF){
		t = gettoken();
		tempStorage[i].value = t;
		curr -> next = &tempStorage[i];
		curr = &tempStorage[i];
		i++;
		tempStorage = (struct node*) realloc(tempStorage,(i + 1) * sizeof(struct node));
	}
	struct node *iter = &tempStorage[0];
	while(iter -> next != NULL){
		printf("%s, lexeme: %s\n", tokennames[iter -> value.id], iter -> value.lexeme);
		iter = iter -> next;
	}
	free(tempStorage);
	return 0;
}
