#ifndef ID_INFO_LINK_NODE
#define ID_INFO_LINK_NODE

/* A linked list IDInfoLinkNode */
typedef struct IDInfoLinkNode
{
	ID_Information* data;
	struct IDInfoLinkNode* next;
} IDInfoLinkNode;

IDInfoLinkNode** push(IDInfoLinkNode** head_ref, ID_Information* ID_to_dd);
IDInfoLinkNode** reverse(IDInfoLinkNode** head_ref);
IDInfoLinkNode* listsConcat(IDInfoLinkNode* head_list, IDInfoLinkNode* tail_list);
IDInfoLinkNode* makeLink(ID_Information* ID_info);

#endif