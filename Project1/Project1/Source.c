#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int poeni;
}Node;

void printi(Node* str) {
	printf("%d", str->poeni);
}

void setValue(Node** str, int points) {
	str = malloc(sizeof(Node));
	printf("%p posle dodele \n", *str);
	(*str)->poeni = points;
}

int main()
{
	Node* a = NULL;
	printf("%p pre funkcije\n", a);
	setValue(&a, 10);
	printf("%p posle funkcije\n", a);
	printf("%d", a->poeni);
}

void try() {
	int x = 5;
	int* c = &x;
	printf("%d\n", &x);
	printf("%d\n", &c);
	printf("%d", c);
}