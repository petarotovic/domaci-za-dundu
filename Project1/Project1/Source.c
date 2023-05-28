#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char ime[30];
	char index[8];
	int poeni;
}Node;

int main()
{
	FILE* file;
	file = fopen("test.txt", "r");
	Node* osoba = NULL;

	if (file == NULL) {
		printf("NE POSTOJI");
		return 1;
	}

	char line[200];
	char ime[30] = "";

	while (fgets(line, sizeof(line), file) != NULL) {
		Node* newOsoba = malloc(sizeof(Node));
		ime[0] = '\0';
		int currentIndex = 0;

		for (int i = 0; i < strlen(line); i++) {
			if (line[i] == ' ' && line[i + 1] == '|') {
				currentIndex = i + 3;
				break;
			}

			char temp[2] = { line[i], '\0' };
			strcat(ime, temp);
		}

		strcpy(newOsoba->ime, ime);

		char index[8] = "";
		
		for (int i = currentIndex; i < strlen(line); i++) {
			if (line[i] == ' ' && line[i + 1] == '-') {
				currentIndex = i + 3;
				break;
			}

			char temp[2] = { line[i], '\0' };
			strcat(index, temp);
		}

		strcpy(newOsoba->index, index);

		for (int i = currentIndex; i < strlen(line); i++) {
			printf("%c", line[i]);
		}
	}

	fclose(file);
	return 0;
}