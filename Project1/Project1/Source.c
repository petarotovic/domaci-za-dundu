#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char ime[30];
	char index[15];
	int poeni;
}Node;

int main()
{
	FILE* file;
	file = fopen("test.txt", "r");
	Node* osoba = malloc(sizeof(Node));

	if (file == NULL) {
		printf("NE POSTOJI");
		return 1;
	}

	char format[100] = "%[^|]|%[^-]-%d";

	fscanf(file, format, osoba->ime, osoba->index, &osoba->poeni);

	printf("%s\n", osoba->ime);
	printf("%s\n", osoba->index);
	printf("%d", osoba->poeni);
}