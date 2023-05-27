#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char ime[30];
    char prezime[30];
    char index[7];
    int poeni;
    struct Node* next;
}Node;

void WriteList(Node* list)
{
    FILE* file;
    Node* current;

    file = fopen("rezultati.txt", "w");

    current = list;
    while (current != NULL) {
        fprintf(file, "%s %s (%s)-%d\n", current->ime, current->prezime, current->index, current->poeni);
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
    while (!feof(file)) {

        if (*head == NULL)
        {
            *head = malloc(sizeof(Node));
            fscanf(file, "%s %s | %s %d", (*head)->ime, (*head)->prezime, (*head)->index, &(*head)->poeni);
            (*head)->next = NULL;
            prev = *head;
        }
        else
        {
            curr = malloc(sizeof(Node));
            fscanf(file, "%s %s | %s %d", curr->ime, curr->prezime, curr->index, &curr->poeni);
            prev->next = curr;
            curr->next = NULL;
            prev = curr;
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
        if (!IsInList(*first, current))
        {
            temp = current->next;
            current->next = *first;
            *first = current;
        }
        else temp = current->next;

        current = temp;
    }
}

int main()
{
    FILE* prviFajl;
    FILE* drugiFajl;

    char firstFile[50], secondFile[50];
    Node* firstList = NULL, * secondList = NULL;


    printf("Unesi ime prve datoteke: ");
    scanf("%s", firstFile);

    printf("Unesi ime druge datoteke: ");
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
}