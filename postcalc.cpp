#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>

struct node
{
	std::string data;
	struct node* prev;
	struct node* next;
};

void welcomemessage()
{
	std::cout << "Hi please use my program correctly" << std::endl;
}

void print(struct node* head)
{
	while (head != NULL)
	{
		std::cout << head->data << std::endl;
		head = head->next;
	}
}

void append(struct node** headref, std::string value)
{
	struct node* newnode = new struct node;
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

void remove(struct node** headref, struct node* subject)
{
	if (*headref == NULL || subject == NULL)
		return;

	if (*headref == subject)
		*headref = subject->next;
	
	if (subject->next != NULL)
		subject->next->prev = subject->prev;

	if (subject->prev != NULL)
		subject->prev->next = subject->next;

	delete subject;
}

std::string operate(struct node** low, struct node** mid, struct node** high)
{
	int firstnum; 
	int secondnum;
	int answer;
	std::string answerstring;
	std::stringstream first((*low)->data); //Convert strings to integers and perform operation
	std::stringstream second((*mid)->data);
	first >> firstnum;
	second >> secondnum;

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

	answerstring = std::to_string(answer); // Convert answer back to string

	return answerstring;
}

std::string traverse(struct node* head, std::string finalanswer)
{
	struct node* current = head;

	while (current->next != NULL)
	{
		if (current->next->data == "+" || current->next->data == "-" || // Perform operation
		current->next->data == 	"x" || current->next->data == "/" || 
		current->next->data == "%")
		{
			finalanswer = operate(&(current->prev), &current, &(current->next));
			remove(&head, current->prev); // Delete previous and next nodes
			remove(&head, current->next);
			current->data = finalanswer; // Replace current node data w/ simplification

			finalanswer = traverse(head, finalanswer); // Recursively traverse expression
		}
		else
			current = current->next;
	}

	return finalanswer;
}

int main(int argc, char* argv[])
{
	struct node* head = NULL;

	if (argc < 4)
	{
		welcomemessage();
		return 0;
	}
	for (int i=1; i < argc; i++) // Add arguments to doubly-linked list
		append(&head, argv[i]);
	
	std::cout << traverse(head, "") << std::endl;

	return 0;
}
