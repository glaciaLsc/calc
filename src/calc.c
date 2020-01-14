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
	struct node* next;
};

/* Debugging */
void print(struct node* head)
{
	printf("Current linked list structure:\n");
	while (head != NULL)
	{
		if (head->data == " " || head->data == "")
			printf("void ", head->data);
		else
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
		*headref = newnode;
	else
	{
		while (current->next != NULL)
			current = current->next;

		current->next = newnode;
	}
}

void erase(struct node* head, struct node* subject)
{
	if (head == subject)
	{
		if (head->next == NULL) return;

		head->data = head->next->data;
		subject = head->next;
		head->next = head->next->next;

		free(subject);
		return;
	}
	
	struct node* prev = head;

	while (prev->next != NULL && prev->next != subject)
		prev = prev->next;

	if (prev->next == NULL) return;

	prev->next = prev->next->next;

	free(subject);
	return;
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
	
	printf("Answer: %d\n", answer);	
	return answer;
}

/* Currently only performs calculations with post-order notation.
 * TODO: Test post-order capability, implement pre-order function
 */
void traversepostorder(struct node* head)
{
	if (head == NULL || head->next == NULL || head->next->next == NULL) return;

	struct node* current = head;
	printf("Current node value(should be head): %s\n", current->data);

	while (current != NULL && current->next != NULL && current->next->next != NULL)
	{
		printf("In while loop...\n");
		if (strcmp(current->next->next->data, "+") == 0 || 
			strcmp(current->next->next->data, "-") == 0 || 
			strcmp(current->next->next->data, "x") == 0 || 
			strcmp(current->next->next->data, "/") == 0 || 
			strcmp(current->next->next->data, "%") == 0)
		{
			printf("In if block...\n");
			sprintf(current->data, "%d", operate(&current, &(current->next), &(current->next->next))); // Call operate function & cast to current data
			printf("Conducted integer to string conversion...\n");
			erase(head, current->next->next); // Delete two next nodes
			printf("Deleted next next...\n");
			print(head);
			erase(head, current->next);
			printf("Deleted next...\n");
			print(head);
			traversepostorder(head);
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
	int i;

	for (i=1; i < argc; i++)
		append(&head, argv[i]);
	
	print(head);
	traversepostorder(head);
	print(head);

	return 0;
}
