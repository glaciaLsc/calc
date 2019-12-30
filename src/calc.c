#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Boolean data type */
enum boolean
{
	false = 0,
	true = 1,
};

typedef enum boolean bool;

/* Argument values (for now, post_order true by default) */
bool post_order = false;
bool pre_order = true;
bool verbose = false;

/* Data structure & operations */
struct node
{
	char *data;
	struct node* prev;
	struct node* next;
};

/* Debugging */
void print(struct node* head)
{
	printf("Current linked list structure:\n");
	while (head != NULL)
	{
		printf("%s  ", head->data);
		head = head->next;
	}
	printf("\n");
}

void append(struct node** headref, char *value)
{
	struct node* newnode = (struct node *)malloc(sizeof(struct node));
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
int operate(struct node** low, struct node** mid, struct node** high)
{
	int firstnum; 
	int secondnum;
	int answer;
	firstnum = atoi((*low)->data); //Convert strings to integers and perform operation
	secondnum = atoi((*mid)->data);

	if (strcmp((*high)->data, "+") == 0)
		answer = firstnum + secondnum;
	else if (strcmp((*high)->data, "-") == 0)
		answer = firstnum - secondnum;
	else if (strcmp((*high)->data, "x") == 0)
		answer = firstnum * secondnum;
	else if (strcmp((*high)->data, "/") == 0)
		answer = firstnum / secondnum;
	else if (strcmp((*high)->data, "%") == 0)
		answer = firstnum % secondnum;
	
	return answer;
}

/* Currently only performs calculations with post-order notation.
 * TODO: Test post-order capability, implement pre-order function
 */
void traverse(struct node* head)
{
	struct node* current = head;

	while (current->next != NULL)
	{
		if (strcmp(current->next->data, "+") == 0 || strcmp(current->next->data, "-") == 0 || 
			strcmp(current->next->data, "x") == 0 || strcmp(current->next->data, "/") == 0 || 
			strcmp(current->next->data, "%") == 0)
		{
			/* Call operate function & cast to current data */
			sprintf(current->data, "%d", operate(&(current->prev), &current, &(current->next)));
			erase(&head, current->prev); // Delete previous and next nodes
			erase(&head, current->next);
			traverse(head); // Recursively traverse expression
		}
		else
			current = current->next;
	}
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

/* Check arguments */
void checkargs(int argc, char* argv[])
{
	if (argc < 3)
	{
		helpdisplay();
		exit(1);
	}
	
	for (int i=0; i < argc; i++)
	{
		if (argv[i] == "-Po" || argv[i] == "--post-order")
			post_order = true;
		else if (argv[i] == "-Pr" || argv[i] == "--pre-order")
			pre_order = true;
		else if (argv[i] == "-v" || argv[i] == "--verbose")
			verbose = true;
	}
}

int main(int argc, char* argv[])
{
	checkargs(argc, argv);

	struct node* head = NULL;

	for (int i=1; i < argc; i++)
		append(&head, argv[i]);

	traverse(head);
	printf("%s\n", head->next->data);

	return 0;
}
