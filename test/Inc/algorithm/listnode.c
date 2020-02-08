
#include "listnode.h"

list* listCreate(void)
{
	struct list* list;
	
	if((list = malloc(sizeof(*list))) == NULL)
	{
		return NULL;
	}
	
	list->head  = list->tail = NULL;
	list->len   = 0;
	list->dup   = NULL;
	list->free  = NULL;
	list->match = NULL; 
	
	return list;
}

void listRelease(list* list)
{
	unsigned long len;
	listNode* current;
	listNode* next;
	
	len = list->len;
	current = list->head;
	
	while(len--)
	{
		next = current->next;
		if(list->free)
		{
			list->free(current->value);
		}
		free(current);
		
		current = next;
	}
	
	free(list);
}

list* listAddNodeHead(list* list, void* value)
{
	listNode* node;
	
	if((node = malloc(sizeof(*node)))  == NULL)
	{
		return NULL;
	}
	node->value = value;
	
	if(list->len == 0)
	{
		list->head = list->tail = node;
		node->prev = node->next = NULL;
	}
	else
	{
		node->prev = NULL;
		node->next = list->head;
		
		list->head->prev = node;
		list->head = node;
	}
	list->len++;
	
	return list;
}

list* listAddNodeTail(list* list, void* value)
{
	listNode* node;
	
	if((node = malloc(sizeof(*node))) == NULL)
	{
		return NULL;
	}
	node->value = value;
	
	if(list->len == 0)
	{
		list->head = list->tail = node;
		node->prev = node->next = NULL;
	}
	else
	{
		node->prev = list->tail;
		node->next = NULL;
		
		list->tail->next = node;
		list->tail = node;
	}
	list->len++;
	
	return list;
}

list* listInsertNode(list* list, listNode* old_node, void* value, int after)
{
	listNode* node;
	
	if((node = malloc(sizeof(*node))) == NULL)
	{
		return NULL;
	}
	node = value;

	if(after)
	{
		node->prev = old_node;
		node->next = old_node->next;
		
		if(list->tail == old_node)
		{
			list->tail = node;
		}
	}
	else
	{
		node->prev = old_node->prev;
		node->next = old_node;
		
		if(list->head == old_node)
		{
			list->head = node;
		}
	}
	
	if(node->prev != NULL)
	{
		node->prev->next = node;
	}
	
	if(node->next != NULL)
	{
		node->next->prev = node;
	}
	list->len++;
	
	return list;
}

void listDelNode(list* list, listNode* node)
{
	if(node->prev)
	{
		node->prev->next = node->next;
	}
	else
	{
		list->head = node->next;
	}
	
	if(node->next)
	{
		node->next->prev = node->prev;
	}
	else
	{
		list->tail = node->prev;
	}
	
	if(list->free)
	{
		list->free(node->value);
	}
	free(node);
	
	list->len--;
}

listIter* listGetIterator(list* list, int direction)
{
	listIter* iter;
	
	if((iter = malloc(sizeof(*iter))) == NULL)
	{
		return NULL;
	}
	
	if(direction == 0)
	{
		iter->next = list->head;
	}
	else
	{
		iter->next = list->tail;
	}
	iter->direction = direction;
	
	return iter;
}

void listReleaseIterator(listIter* iter)
{
	free(iter);
}

void listRewindHead(list* list, listIter* iter)
{
	iter->next = list->head;
	iter->direction = 0;
}

void listRewindTail(list* list, listIter* iter)
{
	iter->next = list->tail;
	iter->direction = 1;
}

listNode* listNext(listIter* iter)
{
	listNode* current  = iter->next;
	if(current != NULL)
	{
		if(iter->direction == 0)
		{
			iter->next = current->next;
		}
		else
		{
			iter->next = current->prev;
		}
	}

	return current;
}

list* listDup(list* orig)
{
	list* copy;
	listNode* node;
	listIter* iter;
	
	if((copy = listCreate()) == NULL)
	{
		return NULL;
	}
	
	copy->dup  = orig->dup;
	copy->free = orig->free;
	copy->match= orig->match;
	
	iter = listGetIterator(orig, 0);
	while((node = listNext(iter)) != NULL)
	{
		void* value;
		
		if(copy->dup)
		{
			value = copy->dup(node->value);
			if(value == NULL)
			{
				listRelease(copy);
				listReleaseIterator(iter);
				
				return NULL;
			}
		}
		else
		{
			value = node->value;
		}
		
		if(listAddNodeTail(copy, value) == NULL)
		{
			listRelease(copy);
			listReleaseIterator(iter);
			
			return NULL;
		}
	}
	
	listReleaseIterator(iter);
	return copy;
}

listNode* listSearchKey(list* list, void* key)
{
	listNode* node;
	listIter* iter;
	
	iter = listGetIterator(list, 0);
	while((node = listNext(iter)) != NULL)
	{
		if(list->match)
		{
			if (list->match(node->value, key))
			{
				listReleaseIterator(iter);
				return node;
			}
		}
		else
		{
			if (key == node->value)
			{
				listReleaseIterator(iter);
				return node;
			}
		}
	}
	listReleaseIterator(iter);
	return NULL;
}

listNode* listIndex(list* list, long index)
{
	listNode* node = NULL;

	if (index < 0)
	{
		index = (-index)-1;

		node = list->tail;
		while(index-- && node)
		{
			node = node->prev;
		}
	}
	else
	{
		node = list->head;
		while(index-- && node)
		{
			node = node->next;
		}
	}

	return node;
}

void listRotate(list* list)
{
	listNode* node = list->tail;

	if (listLength(list) <= 1)
	{
		return;
	}

	list->tail = node->prev;
	list->tail->next = NULL;

	list->head->prev = node;
	node->prev = NULL;
	node->next = list->head;
	list->head = node;
}