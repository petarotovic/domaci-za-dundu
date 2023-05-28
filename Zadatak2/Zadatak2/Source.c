#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char ime[30];
    char index[8];
    int poeni;
    struct Node* next;
}Node;

void FreeLinkedList(Node* head) {
    Node* current = head;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

void WriteList(Node* list)
{
    FILE* file;
    Node* current;

    file = fopen("rezultati.txt", "w");

    current = list;
    while (current != NULL) {
        fprintf(file, "%s ( %s ) - %d\n", current->ime, current->index, current->poeni);
        current = current->next;
    }
    fclose(file);
}

void SortLinkedListByIndex(Node** head) {
    int swapped;
    Node* current;
    Node* last = NULL;
    Node* temp = NULL;

    if (*head == NULL)
        return;
    if ((*head)->next == NULL)
        return;

    do {
        swapped = 0;
        current = *head;

        while (current->next != last) {
            temp = current->next;
            if (strcmp(current->index, temp->index) > 0) {
                Node* temp = current->next;
                current->next = temp->next;
                temp->next = current;

                if (current == *head) {
                    *head = temp;
                }
                else {
                    Node* prev = *head;
                    while (prev->next != current) {
                        prev = prev->next;
                    }
                    prev->next = temp;
                }

                current = temp;
                swapped = 1;
            }

            current = current->next;
        }

        last = current;
    } while (swapped);
}

void SetValues(Node** node, char* line) {
    char ime[31] = "";
    int currentIndex = 0;

    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ' && line[i + 1] == '|') {
            currentIndex = i + 3;
            break;
        }

        char temp[2] = { line[i], '\0' };
        strcat(ime, temp);
    }

    strcpy((*node)->ime, ime);

    char index[8] = "";

    for (int i = currentIndex; i < strlen(line) - 1; i++) {
        if (line[i] == ' ' && line[i + 1] == '-') {
            currentIndex = i + 3;
            break;
        }

        char temp[2] = { line[i], '\0' };
        strcat(index, temp);
    }

    strcpy((*node)->index, index);

    char poeni[10] = "";

    for (int i = currentIndex; i < strlen(line); i++) {
        if (line[i] == '\n' || line[i] == EOF) {
            break;
        }

        char temp[2] = { line[i], '\0' };
        strcat(poeni, temp);
    }

    (*node)->poeni = atoi(poeni);
}

int LoadList(const char* firstFile, Node** head)
{
    FILE* file;
    Node* prev, * curr;

    file = fopen(firstFile, "r");

    if (file == NULL)
    {
        return 0;
    }

    prev = *head;
    char line[200];
    char ime[30] = "";

    while (fgets(line, sizeof(line), file) != NULL) {

        if (*head == NULL)
        {
            *head = malloc(sizeof(Node));
            
            SetValues(head, line);

            (*head)->next = NULL;

            prev = *head;
            // free(*head);
        }
        else
        {
            curr = malloc(sizeof(Node));

            SetValues(&curr, line);

            prev->next = curr;
            curr->next = NULL;
            prev = curr;
            // free(curr);
        }
    }

    fclose(file);

    return 1;
}

int IsInList(Node* list, Node* node)
{
    Node* current;
    current = list;
    while (current != NULL)
    {
        if (strcmp(current->index, node->index) == 0)
        {
            current->poeni += node->poeni;
            return 1;
        }
        current = current->next;
    }
    return 0;
}


void AddPointsAndConcat(Node** first, Node* second)
{
    Node* current;
    Node* temp;

    current = second;
    while (current != NULL)
    {
        if (!IsInList((*first), current))
        {
            temp = current->next;
            current->next = *first;
            *first = current;
        }
        else
        {
            temp = current->next;
            free(current);
        }

        current = temp;
    }
}

int main()
{
    FILE* prviFajl;
    FILE* drugiFajl;

    char firstFile[31] = "dz1.txt", secondFile[31] = "dz2.txt";
    Node* firstList = NULL, * secondList = NULL;

    scanf("%s", firstFile);

    scanf("%s", secondFile);

    if (!LoadList(firstFile, &firstList))
    {
        printf("DAT_GRESKA");
        return 0;
    }
    if (!LoadList(secondFile, &secondList))
    {
        printf("DAT_GRESKA");
        return 0;
    }

    AddPointsAndConcat(&firstList, secondList);

    SortLinkedListByIndex(&firstList);

    WriteList(firstList);

    FreeLinkedList(firstList);

    return 0;
}