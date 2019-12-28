#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Boolean data type */
enum boolean
{
	false = 0,
	true = 1,
};

typedef enum boolean bool;

/* Argument values */
bool post_order;
bool pre_order;
bool verbose;

/* Data structure & operations */
struct node
{
	char* data;
	struct node* prev;
	struct node* next;
};

void append(struct node** headref, char *value)
{
	struct node* newnode;
	struct node* current = *headref;
	
	newnode->data = value;
	newnode->next = NULL;

	if (*headref == NULL)
	{
		newnode->prev = NULL;
		*headref = newnode;
	}
	else
	{
		while (current->next != NULL)
			current = current->next;

		current->next = newnode;
		newnode->prev = current;
	}

	free(newnode);
}

void erase(struct node** headref, struct node* subject)
{
	if (*headref == NULL || subject == NULL)
		return;

	if (*headref == subject)
		*headref = subject->next;
	
	if (subject->next != NULL)
		subject->next->prev = subject->prev;

	if (subject->prev != NULL)
		subject->prev->next = subject->next;

	free(subject);
}

/* Perform operation with two operands and an operator */
char *operate(struct node** low, struct node** mid, struct node** high)
{
	int firstnum; 
	int secondnum;
	int answer;
	char *answerstring;
	firstnum = atoi((*low)->data); //Convert strings to integers and perform operation
	secondnum = atoi((*mid)->data);

	if ((*high)->data == "+")
		answer = firstnum + secondnum;
	else if ((*high)->data == "-")
		answer = firstnum - secondnum;
	else if ((*high)->data == "x")
		answer = firstnum * secondnum;
	else if ((*high)->data == "/")
		answer = firstnum / secondnum;
	else if ((*high)->data == "%")
		answer = firstnum % secondnum;

	*answerstring = answer; // Convert answer back to string

	return answerstring;
}

/* Currently only performs calculations with post-order notation.
 * TODO: Test post-order capability, implement pre-order function
 */
char *traverse(struct node* head, char *finalanswer)
{
	struct node* current = head;

	while (current->next != NULL)
	{
		if (current->next->data == "+" || current->next->data == "-" || // Perform operation
		current->next->data == 	"x" || current->next->data == "/" || 
		current->next->data == "%")
		{
			finalanswer = operate(&(current->prev), &current, &(current->next));
			erase(&head, current->prev); // Delete previous and next nodes
			erase(&head, current->next);
			current->data = finalanswer; // Replace current node data w/ simplification

			finalanswer = traverse(head, finalanswer); // Recursively traverse expression
		}
		else
			current = current->next;
	}

	return finalanswer;
}

/* Print functions */
void helpdisplay()
{
	printf("Usage: calc [OPTION]... [DATA]...\n");
	printf("Perform basic calculations on the command line. In-order input accepted by default.\n\n");
	printf("-Po, --post-order      accept post-order input\n");
	printf("-Pr, --pre-order       accept pre-order input\n");
	printf("-v,  --verbose         verbose output\n");
}

int main(int argc, char* argv[])
{
	helpdisplay();

	return 0;
}
