#include <stdio.h>
#include <stdlib.h>
#include "SymbolTable.h"


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

IDInfoLinkNode* listsConcat(IDInfoLinkNode* head_list, IDInfoLinkNode* tail_list) {
	
	if (head_list == NULL && tail_list == NULL)
		return NULL;
	
	if (head_list == NULL)
		return tail_list;

	if (tail_list == NULL)
		return head_list;

	IDInfoLinkNode* head = head_list;
	while (head_list->next != NULL)
		head_list = head_list->next;
	head_list->next = tail_list;

	return head;
}

IDInfoLinkNode* makeLink(ID_Information* ID_info) {
	IDInfoLinkNode* new_node = (struct IDInfoLinkNode*)malloc(sizeof(struct IDInfoLinkNode));
	if (new_node != NULL)
	{
		new_node->data = ID_info;
		new_node->next = NULL;
		return new_node;
	}
	return NULL;
}