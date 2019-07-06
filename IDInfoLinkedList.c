#include <stdio.h>
#include <stdlib.h>
#include "SymbolTable.h"

/* A linked list IDInfoLinkNode */
typedef struct IDInfoLinkNode
{
	ID_Information* data;
	struct IDInfoLinkNode* next;
} IDInfoLinkNode;

/* Function to add a IDInfoLinkNode at the beginning of Linked List.
   This function expects a pointer to the data to be added. */
IDInfoLinkNode** push(IDInfoLinkNode** head_ref, ID_Information* ID_to_dd)
{
	// Allocate memory for IDInfoLinkNode 
	struct IDInfoLinkNode* new_node = (struct IDInfoLinkNode*)malloc(sizeof(struct IDInfoLinkNode));

	new_node->data = ID_to_dd;
	new_node->next = (*head_ref);

	// Change head pointer as new IDInfoLinkNode is added at the beginning 
	(*head_ref) = new_node;

	return head_ref;
}
IDInfoLinkNode** reverse(IDInfoLinkNode** head_ref) {
	struct IDInfoLinkNode* prev = NULL;
	struct IDInfoLinkNode* current = *head_ref;
	struct IDInfoLinkNode* next;

	while (current != NULL) {
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}

	*head_ref = prev;
	return head_ref;
}
IDInfoLinkNode** listsConcat(IDInfoLinkNode** head_list, IDInfoLinkNode** tail_list) {
	// TODO: implement!
	// if tail is null return list that contain only the head.
	// check that the head is not null.
	return NULL;
}