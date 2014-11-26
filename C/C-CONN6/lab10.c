//
// APS105-Fall 2014 Lab 10 lab10.c
//
// Program for maintaining a personal phone book.
//
// Uses a linked list to hold the phone book entries.
//
// Author: <Your Name Goes Here>
// Student Number: <Your Student Number Goes Here>
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

//**********************************************************************
// Linked List Definitions 
//  Define your linked list node and pointer types
//  here for use throughout the file.
//
//   ADD STATEMENT(S) HERE

#define MAX	50

typedef struct Node {
	char p_fa_name[MAX]; /* the key */
	char p_fi_name[MAX];
	char p_address[MAX];
	char p_telno[MAX];

	struct Node *next;
	struct Node *prev;
} person;

typedef struct List {
	person *header;
	person *tailer;

	int count;
} bookphone;

//**********************************************************************
// Linked List Function Declarations
//
// Functions that modify the linked list.
//   Declare your linked list functions here.
//
//   ADD STATEMENT(S) HERE

int bookInsert(person *p, bookphone *pb);
int bookDelete(char *fa_name, bookphone *pb);
person* bookSearchByName(char *fa_name, bookphone *pb);
person* bookRsearchByPhone(char *phone, bookphone *pb);
void bookPrintPerson(person *p);
void bookDisplay(bookphone *pb);
void bookDestroy(bookphone *pb);

//**********************************************************************
// Support Function Declarations
//

void safegets (char s[], int arraySize);        // gets without buffer overflow
void familyNameDuplicate (char familyName[]);   // marker/tester friendly 
void familyNameFound (char familyName[]);       //   functions to print
void familyNameNotFound (char familyName[]);    //     messages to user
void familyNameDeleted (char familyName[]);
void phoneNumberFound (char phoneNumber[]);
void phoneNumberNotFound (char phoneNumber[]);
void printPhoneBookEmpty (void);
void printPhoneBookTitle (void);

//**********************************************************************
// Program-wide Constants
//

const int MAX_LENGTH = 1023;
const char NULL_CHAR = '\0';
const char NEWLINE = '\n';

//**********************************************************************
// Main Program
//

int main (void)
{ 
    const char bannerString[]
        = "Personal Phone Book Maintenance Program.\n\n";
    const char commandList[]
        = "Commands are I (insert), D (delete), S (search by name),\n"
          "  R (reverse search by phone #), P (print), Q (quit).\n";

    // Declare linked list head.
    //   ADD STATEMENT(S) HERE TO DECLARE LINKED LIST HEAD.
	person p;
	bookphone pbk;
	bookphone *pb;

	/* init */
	pbk.header = NULL;
	pbk.tailer = NULL;
	pbk.count = 0;

	pb = &pbk;
 
    // announce start of program
    printf("%s",bannerString);
    printf("%s",commandList);
    
    char response;
    char input[MAX_LENGTH+1];
    do
    {
        printf("\nCommand?: ");
        safegets(input,MAX_LENGTH+1);
        // Response is first char entered by user.
        // Convert to uppercase to simplify later comparisons.
        response = toupper(input[0]);

        if (response == 'I')
        {
            // Insert an phone book entry into the linked list.
            // Maintain the list in alphabetical order by family name.
            //   ADD STATEMENT(S) HERE

            // USE THE FOLLOWING PRINTF STATEMENTS WHEN PROMPTING FOR DATA:
            printf("  family name: ");
			safegets(p.p_fa_name, MAX+1);
            printf("  first name: ");
			safegets(p.p_fi_name, MAX+1);
            printf("  address: ");
			safegets(p.p_address, MAX+1);
            printf("  phone number: ");
			safegets(p.p_telno, MAX+1);

			bookInsert(&p, pb);
        }
        else if (response == 'D')
        {
            // Delete an phone book entry from the list.

            printf("\nEnter family name for entry to delete: ");

            //   ADD STATEMENT(S) HERE
			safegets(p.p_fa_name, MAX+1);

			if (bookDelete(p.p_fa_name, pb) == 0)
				familyNameDeleted(p.p_fa_name);

        }
        else if (response == 'S')
        {
            // Search for an phone book entry by family name.

            printf("\nEnter family name to search for: ");

            //   ADD STATEMENT(S) HERE
			safegets(p.p_fa_name, MAX+1);
			person *pr = bookSearchByName(p.p_fa_name, pb);
			
			if (NULL == pr) {
				familyNameNotFound(p.p_fa_name);
			}
			else {
				familyNameFound(p.p_fa_name);
				printf("\n");
				bookPrintPerson(pr);
			}
        }
        else if (response == 'R')
        {
            // Search for an phone book entry by phone number.

            printf("\nEnter phone number to search for: ");

            //   ADD STATEMENT(S) HERE
			safegets(p.p_telno, MAX+1);
			person *pr = bookRsearchByPhone(p.p_telno, pb);

			if (NULL == pr) {
				phoneNumberNotFound(p.p_telno);
			}
			else {
				phoneNumberFound(p.p_telno);
				printf("\n");
				bookPrintPerson(pr);
			}
        }
        else if (response == 'P')
        {
            // Print the phone book.
            //   ADD STATEMENT(S) HERE

			bookDisplay(pb);

        }
        else if (response == 'Q')
        {
            ; // do nothing, we'll catch this below
        }
        else 
        {
            // do this if no command matched ...
            printf("\nInvalid command.\n%s\n",commandList);
        }
    } while (response != 'Q');
  
    // Delete the whole phone book linked list.
    //   ADD STATEMENT(S) HERE
	bookDestroy(pb);

    // Print the linked list to confirm deletion.
    //   ADD STATEMENT(S) HERE
	bookDisplay(pb);

    return 0;
}

//**********************************************************************
// Support Function Definitions

// Function to get a line of input without overflowing target char array.
void safegets (char s[], int arraySize)
{
    int i = 0, maxIndex = arraySize-1;
    char c;
    while (i < maxIndex && (c = getchar()) != NEWLINE)
    {
        s[i] = c;
        i = i + 1;
    }
    s[i] = NULL_CHAR;
}

// Function to call when user is trying to insert a family name 
// that is already in the book.
void familyNameDuplicate (char familyName[])
{
    printf("\nAn entry for <%s> is already in the phone book!\n"
           "New entry not entered.\n",familyName);
}

// Function to call when a family name was found in the phone book.
void familyNameFound (char familyName[])
{
    printf("\nThe family name <%s> was found in the phone book.\n",
             familyName);
}

// Function to call when a family name was not found in the phone book.
void familyNameNotFound (char familyName[])
{
    printf("\nThe family name <%s> is not in the phone book.\n",
             familyName);
}

// Function to call when a family name that is to be deleted
// was found in the phone book.
void familyNameDeleted (char familyName[])
{
    printf("\nDeleting entry for family name <%s> from the phone book.\n",
             familyName);
}

// Function to call when a phone number was found in the phone book.
void phoneNumberFound (char phoneNumber[])
{
    printf("\nThe phone number <%s> was found in the phone book.\n",
             phoneNumber);
}

// Function to call when a phone number was not found in the phone book.
void phoneNumberNotFound (char phoneNumber[])
{
    printf("\nThe phone number <%s> is not in the phone book.\n",
             phoneNumber);
}

// Function to call when printing an empty phone book.
void printPhoneBookEmpty (void)
{
    printf("\nThe phone book is empty.\n");
}

// Function to call to print title when whole phone book being printed.
void printPhoneBookTitle (void)
{
    printf("\nMy Personal Phone Book: \n");
}

//**********************************************************************
// Add your functions below this line.
//   ADD STATEMENT(S) HERE

int bookInsert(person *p, bookphone *pb)
{
	if (bookSearchByName(p->p_fa_name, pb) != NULL) {
		familyNameDuplicate(p->p_fa_name);
		return -1;
	}

	person *np = (person*) malloc(sizeof(person));
	*np = *p;
	np->next = np->prev = np;

	if (pb->header == NULL) {
		pb->header = pb->tailer = np;
	}
	else {
		person *itr = pb->header;
		while (strcmp(np->p_fa_name, itr->p_fa_name) > 0) {
			itr = itr->next;
			if (itr == pb->header) break;
		}

		if (pb->header == itr 
				&& strcmp(np->p_fa_name, pb->tailer->p_fa_name) > 0) {
			/* add to tailer */
			np->prev = pb->tailer;
			np->next = pb->tailer->next;

			pb->tailer->next = np;
			pb->tailer = np;
		}
		else {
			/* add to itr's ahead */
			person *pitr = itr->prev;

			pitr->next->prev = np;
			np->next = pitr->next;

			pitr->next = np;
			np->prev = pitr;

			/* np is new header */
			if (itr == pb->header)
				pb->header = np;
		}
	}
	pb->count++;
}

person* bookSearchByName(char *fa_name, bookphone *pb)
{
	person *itr = pb->header;

	while (NULL != itr && strcmp(itr->p_fa_name, fa_name) != 0)
	{
		itr = itr->next;

		if (itr == pb->header) break;
	}

	if (itr != pb->header || NULL == itr) return itr;

	if (itr == pb->header && strcmp(itr->p_fa_name, fa_name) == 0)
		return itr;

	return NULL;
}

int bookDelete(char *fa_name, bookphone *pb)
{
	if (bookSearchByName(fa_name, pb) == NULL) {
		familyNameNotFound(fa_name);
		return -1;
	}

	person *itr = pb->header;
	while (strcmp(itr->p_fa_name, fa_name) != 0)
		itr = itr->next;

	/* delete node */
	itr->prev->next = itr->next;
	itr->next->prev = itr->prev;

	if (itr == pb->header) pb->header = itr->next;
	if (itr == pb->tailer) pb->tailer = itr->prev;

	free(itr);

	pb->count--;
	if (pb->count == 0) {
		pb->header = pb->tailer = NULL;
	}

	return 0;
}

void bookPrintPerson(person *p)
{
	if (p != NULL) {
		printf("%s\n%s\n%s\n%s\n\n",
				p->p_fa_name, p->p_fi_name,
				p->p_address, p->p_telno);
	}
}

void bookDisplay(bookphone *pb)
{
	if (NULL == pb || pb->count == 0) {
		printPhoneBookEmpty();
	}
	else {
		printPhoneBookTitle();
		
		printf("\n");

		person *itr = pb->header;
		int count = pb->count;
		while (count-- > 0) {
			bookPrintPerson(itr);
			itr = itr->next;
		}
	}
}

person *bookRsearchByPhone(char *telno, bookphone *pb)
{
	person *itr = pb->tailer;

	while (NULL != itr && strcmp(itr->p_telno, telno) != 0)
	{
		itr = itr->prev;
		if (itr == pb->tailer) break;
	}

	if (itr != pb->tailer || itr == NULL) return itr;

	if (itr == pb->tailer && strcmp(itr->p_telno, telno) == 0)
		return itr;

	return NULL;

}

void bookDestroy(bookphone *pb)
{
	person *itr = pb->header;

	int count = pb->count;
	while (count-- > 0) {
		pb->header = itr->next;
		free(itr);
		itr = pb->header;
	}

	pb->header = pb->tailer = NULL;
	pb->count = 0;
}
