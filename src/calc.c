#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Boolean data type */
typedef enum {false, true} bool;

/* Node data structure */
struct node
{
	char *data;
	struct node* next;
};

// Flags
bool post_order = false;
bool pre_order = false;
bool verbose = false;

/* Linked list operations */
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

    //Convert strings to integers and perform operation
	firstnum = atoi((*low)->data); 
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

void traversepostorder(struct node* head)
{
	if (head == NULL || head->next == NULL || head->next->next == NULL) return;

	struct node* current = head;

	while (current != NULL && current->next != NULL && current->next->next != NULL)
	{
		if (strcmp(current->next->next->data, "+") == 0 || 
			strcmp(current->next->next->data, "-") == 0 || 
			strcmp(current->next->next->data, "x") == 0 || 
			strcmp(current->next->next->data, "/") == 0 || 
			strcmp(current->next->next->data, "%") == 0)
		{
			// Call operate function & cast to current data
			sprintf(current->data, "%d", operate(&current, &(current->next), &(current->next->next)));

			// Delete next two nodes
			erase(head, current->next->next); 
			erase(head, current->next);
			
			// Recursively traverse linked list
			traversepostorder(head); 
		}
		else
			current = current->next;
	}
}

void traversepreorder(struct node* head)
{
	if (head == NULL || head->next == NULL || head->next->next == NULL) return;

	struct node* current = head;

	while (current != NULL && current->next != NULL && current->next->next != NULL)
	{
		if ((strcmp(current->data, "+") == 0 ||
			strcmp(current->data, "-") == 0 || 
			strcmp(current->data, "x") == 0 ||
			strcmp(current->data, "/") == 0 ||
			strcmp(current->data, "%") == 0) && (
			strcmp(current->next->data, "+") != 0 &&
			strcmp(current->next->data, "-") != 0 &&
			strcmp(current->next->data, "x") != 0 &&
			strcmp(current->next->data, "/") != 0 &&
			strcmp(current->next->data, "%") != 0) && (
			strcmp(current->next->next->data, "+") != 0 && 
			strcmp(current->next->next->data, "-") != 0 && 
			strcmp(current->next->next->data, "x") != 0 && 
			strcmp(current->next->next->data, "/") != 0 && 
			strcmp(current->next->next->data, "%") != 0))
		{
			// Call operate function & cast to current data
			sprintf(current->data, "%d", operate(&(current->next), &(current->next->next), &current));

			// Delete next two nodes
			erase(head, current->next->next); 
			erase(head, current->next);

			// Recursively traverse linked list
			traversepreorder(head); 
		}
		else
			current = current->next;
	}
}

/* Print functions */
void printhelpdisplay()
{
	printf("Usage: calc [OPTION]... [DATA]...\n\n");
	printf("Options:\n");
	printf("-Po, --post-order      accept post-order input\n");
	printf("-Pr, --pre-order       accept pre-order input\n");
	printf("-v,  --verbose         verbose output\n");
}

void printerrordisplay()
{
	printf("ERROR: Improper syntax\n\n");
	printf("Type -h or --help for a list of available commands\n");
}

/*
 * TODO: Make argument checking more efficient
 * */
int checkargs(int argc, char* argv[])
{
	// Check for minimum arg count
	if (argc == 1)
	{
		printerrordisplay();
		exit(-1);
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
		{
			printhelpdisplay();
			exit(0);
		}
		printerrordisplay();
		exit(-1);
	}

	int flagcount = 1;
	
	for (int i=1; i < argc; i++)
	{
		if (strcmp(argv[i], "-Po") == 0 || strcmp(argv[i], "--post-order") == 0)
		{
			post_order = true;
			flagcount++;
		}
		else if (strcmp(argv[i], "-Pr") == 0 || strcmp(argv[i], "--pre-order") == 0)
		{
			pre_order = true;
			flagcount++;
		}
		else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
		{
			verbose = true;
			flagcount++;
		}
	}

	return flagcount;
}

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

/* 
 * TODO: Implement verbose versions of pre-and post-order functions;
 *       implement in-order function
 */
int main(int argc, char* argv[])
{
	// Check arguments for validity and flag count
	int flagcount = checkargs(argc, argv);

	struct node* head = NULL;

	// Add non-flag args to linked list
	for (int i=flagcount; i < argc; i++) 
		append(&head, argv[i]);
	
	// Perform calculation
	if (post_order == true)
	{
		traversepostorder(head);
		printf("%s\n", head->data);
	} 
	else if (pre_order == true)
	{
		traversepreorder(head);
		printf("%s\n", head->data);
	}
	
	return 0;
}
