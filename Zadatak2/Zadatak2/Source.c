#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Zamisli linked listu kao neku kocku. U toj kocki imas neke podatke sto su u nasem slucaju ime, index, poeni
//i zadnji podatak je next. Next pokazuje na sledecu kocku ili linked listu. Preko next joj i pristupamo.

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

//Ova funkcija prima listu, sto ce kod nas da bude ona sredjena lista i upisace je u fajl.
void WriteList(Node* list)
{
    FILE* file;

    //Ovo ce da bude prva kocka u listi
    Node* current;

    file = fopen("rezultati.txt", "w");

    //Ovde postavljam current na da bude prva kocka u listi.
    //Zadnja kocka u listi za next ima NULL
    current = list;

    //Posto zadnja kocka u listi ima vrednost NULL mi radimo nesto sve dok nije NULL
    //Mislim da ovde ne treba nesto posebno da objasnjavam, obican upis u fajl, liniju po liniju
    while (current != NULL) {
        
        fprintf(file, "%s ( %s ) - %d\n", current->ime, current->index, current->poeni);
        current = current->next;
    }
    fclose(file);
}

void WriteEmptyFile() {
    FILE* file;

    file = fopen("rezultati.txt", "w");
    fprintf(file, "");
}

void SortLinkedListByIndex(Node** head) {
    int swapped;
    Node* current;
    Node* last = NULL;
    Node* temp = NULL;


    //Ako je prazna lista, onda samo izbacuje ne ide dalje
    if (*head == NULL)
        return;
    if ((*head)->next == NULL)
        return;

    do {
        //Ovo ce da bude indikator da li je u trenutnoj iteraciji izvrsena zamena mesta kockama
        swapped = 0;

        //Postavljamo trenutnu kocku na prvu u listi
        current = *head;

        //Posto svaka kocka sadrzi pokazivac na sledecu, proveravamo da li pokazivac koji se nalazi u trenutnoj kocki
        //pokazuje na zadnju
        while (current->next != last) {
            //Stavljam u temp sledecu kocku
            temp = current->next;

            //Uporedjujem indekse od trenutne kocke i one posle nje koja je smestena u temp
            if (strcmp(current->index, temp->index) > 0) {
                //Ako je potrebno da se zamene uzimamo pokazivac na sledecu kocku iz current i stavljamo u temp
                Node* temp = current->next;
                //Pokazivac u trenutnu kocku postavljamo na pokazivac od sledece kocke tj. one iz temp
                current->next = temp->next;
                //A pokazivac u temp postavljamo na current i ovako smo zamenili pokazivace na dve kocke tj rotirale su mesta
                temp->next = current;

                //ako je doslo do zamene mesta, sada se nova kocka nalazi na pocetku liste i head postavljamo da bude ta nova kocka
                if (current == *head) {
                    *head = temp;
                }
                else {
                    //u slucaju da 
                    Node* prev = *head;
                    while (prev->next != current) {
                        prev = prev->next;
                    }
                    prev->next = temp;
                }

                //Postavljamo current ili trenutnu kocku da bude nova kocka koja je na pocetku
                current = temp;
                //swapped postavljamo na 1 i to oznacava da je do zamene doslo
                swapped = 1;
            }

            //Postavljamo trenutnu kocku na sledecu u nizu
            current = current->next;
        }

        //Postavljamo zadnju kocku koja je pomerena
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

    fseek(file, 0, SEEK_END);

    long fileSize = ftell(file);

    if (fileSize == 0) {
        fclose(file);
        return 0;
    }

    fseek(file, 0, SEEK_SET);

    prev = *head;

    //Ako je lista prazna
    if (*head == NULL)
    {
       *head = malloc(sizeof(Node));

       if (*head == NULL) {
           printf("MEM_GRESKA");
           return 1;
       }
            
       //Citam samo jedan red iz fajla, tj. prvi
       fscanf(file, " %[^|] | %[^-] - %d", (*head)->ime, (*head)->index, &(*head)->poeni);

       if (!strcmp((*head)->ime, "") || !strcmp((*head)->index, "HHHHHHHH") || (*head)->poeni < -1000 || strchr((*head)->ime, '\n') != NULL) {
           WriteEmptyFile();
           (*head)->next = NULL;
           return 0;
       }

       //Na prvoj kocki postavlja se pokazivac na sledecu da je NULL, jer nista nije jos ucitano za ostale
       (*head)->next = NULL;

       //Postavljam prev na prvu kocku u listu sto je u ovom slucaju head cije su vrednosti setovane u fscanf
       prev = *head;
       // free(*head);
    }

    curr = malloc(sizeof(Node));

    if (curr == NULL) {
        printf("MEM_GRESKA");
        return 1;
    }

    //U slucaju da head tj. lista nije prazna onda citamo sve ostale redove iz fajla
    while (fscanf(file, "%*[\n]%[^|] | %[^-] - %d", curr->ime, curr->index, &curr->poeni) != EOF)
    {
        if (!strcmp(curr->ime, "") || !strcmp(curr->index, "HHHHHHHH") || curr->poeni < -1000 || strchr(curr->ime, '\n') != NULL) {
            WriteEmptyFile();
            (*head)->next = NULL;
            return 0;
        }
        //Na prev postavljamo pokazivac na curr tj. sledecu kocku(red) koju smo procitali iz fajla
        prev->next = curr;
        //Sad ta nova procitana kocka ne pokazuje ni na jednu sledecu dok se ne ucita
        curr->next = NULL;
        //Trenutnu kocku postavljamo na novu ucitanu
        prev = curr;
        //Alociramo novu memoriju za curr
        curr = malloc(sizeof(Node));

        if (curr == NULL) {
            printf("MEM_GRESKA");
            return 1;
        }
    }

    fclose(file);
    free(curr);

    return 1;
}

int IsInList(Node* list, Node* node)
{
    Node* current;

    //postavljamo current da je prva kocka u listi
    current = list;

    //prolazimo kroz listu sve dok pokazivaci na kockama nisu NULL
    while (current != NULL)
    {
        //Ako nadjemo neku kocku gde se slazu indexi onda povecavamo poene na toj kocki
        if (strcmp(current->index, node->index) == 0)
        {
            current->poeni += node->poeni;
            //Sabira poene i vraca true ili 1
            return 1;
        }
        //Ako se ne slazu indexi onda postavljamo current na sledecu kocku
        current = current->next;
    }

    //Ako se ne nalazi u listi vracamo 0
    return 0;
}

//U ovoj funkciji sabiramo poene iz prve i druge liste tj. iz prvog txt fajla i drugog txt fajla
void AddPointsAndConcat(Node** first, Node* second)
{
    Node* current;
    Node* temp;

    //postavljamo current na prvu kocku u drugoj listi
    current = second;

    //Sve dok je kocka u drugoj listi razlicita od NULL
    while (current != NULL)
    {
        //Ako se ne nalazi u listi
        if (!IsInList((*first), current))
        {
            //Sad iz druge liste ubacujemo kocku u prvu listu
            temp = current->next;
            current->next = *first;
            *first = current;
        }
        else
        {
            //Ako se nalazi
            temp = current->next;
            free(current);
        }

        current = temp;
    }

    free(current);
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
        FreeLinkedList(firstList);
        printf("DAT_GRESKA");
        return 0;
    }
    if (!LoadList(secondFile, &secondList))
    {
        FreeLinkedList(firstList);
        FreeLinkedList(secondList);
        printf("DAT_GRESKA");
        return 0;
    }

    AddPointsAndConcat(&firstList, secondList);

    SortLinkedListByIndex(&firstList);

    WriteList(firstList);

    FreeLinkedList(firstList);

    return 0;
}