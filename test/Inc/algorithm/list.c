#include "list.h"

#include <stdio.h>

/*
 * 创建链表节点
*/
List *CreateList(gDataPointer data)
{
    List *pList = (List *)malloc(sizeof(List));
    if (pList == NULL)
    {
        return NULL;
    }
    memset(pList, 0x0, sizeof(List));

    pList->data = data;
    pList->next = NULL;

    return pList;
}

/*
 * 删除链表节点
*/
void DeleteList(List *pList)
{
    if (pList == NULL)
    {
        return;
    }
}

/*
 * 链表末尾增加节点
*/
static List* AppendList(List *pList, gDataPointer data)
{
    List *new_list = NULL;
    List *last = NULL;

    new_list = CreateList(data);
    if (new_list == NULL)
    {
        return new_list;
    }

    if (pList == NULL)
    {
        return new_list;
    }

	last = GetLastList(pList);
	last->next = new_list;

    return pList;
}

/*
 * 链表首部增加节点
*/
static List* PrependList(List *pList, gDataPointer data)
{
    List *new_list = NULL;

    new_list = CreateList(data);
    if (new_list == NULL)
    {
        return NULL;
    }

    new_list->next = pList;
    return new_list;
}

/*
 * 插入链表位置数据
*/
List* InsertList(List *pList, gDataPointer data, int pos)
{
    if (pList == NULL)
    {
        return NULL;
    }

    if (pos == 0)
    {
        return PrependList(pList, data);
    }
    else if (pos < 0)
    {
        return AppendList(pList, data);
    }

    List *new_list = CreateList(data);
    if (new_list == NULL)
    {
        return NULL;
    }

    List *prev = NULL;
    List *head = pList;

    while ((pos-- > 0) && (head != NULL))
    {
        prev = head;
        head = head->next;
    }

    new_list->next = prev->next;
    prev->next = new_list;

    return pList;
}

/*
 * 插入链表节点数据
*/
List* InsertList_before(List *pList, List *pSibing, gDataPointer data)
{
    if (pList == NULL)
    {
        pList = CreateList(data);
        return pList;
    }

    List *last = NULL;  //prev
    List *node = pList;

    while (node != NULL)
    {
        if (node == pSibing)
        {
            break;
        }

        last = node;
        node = last->next;
    }

    node = CreateList(data);
    if (last == NULL)
    {
        node->next = pSibing;
        return node;
    }

    node->next = last->next;
    last->next = node;

    return pSibing;
}

/*
 * 连接两个链表
*/
List* ConCatList(List *pList1, List *pList2)
{
    return pList1;
}

/*
 * 移除链表数据
*/
List* RemoveData(List *pList, gDataPointer data)
{
    List *head = NULL;
    List **prev = &pList;

    while (*prev != NULL)
    {
        head = *prev;
        if (head->data == data)
        {
            *prev = head->next;
            break;
        }

        prev = &head->next;
    }

    return pList;
}

/*
 * 移除链表节点
*/
List* RemoveLink(List *pList, List *pLink)
{
    List *head = NULL;
	List **prev = &pList;

	while (*prev != NULL)
	{
		head = *prev;
		if (head == pLink)
		{
			*prev = head->next;
			head->next = NULL;
			break;
		}

		prev = &head->next;
	}

    return pList;
}

/*
 * 拷贝链表
*/
List* CopyList(List *pList)
{
    return NULL;
}

/*
 * 翻转链表
*/
List* ReverseList(List *pList)
{
    return NULL;
}

/*
 * 查找链表
*/
List* FindList(List *pList)
{
    return NULL;
}

/*
 * 获取链表最后一个节点
*/
List *GetLastList(List *pList)
{
    if (pList != NULL)
    {
        while (pList->next)
        {
            pList = pList->next;
        }
    }

    return pList;
}

/*
 * 获取节点位置
*/
int GetListPos(List *pList, List *pLink)
{
    int iPos = -1;

	while(pList != NULL)
	{
		++iPos;
		if (pList == pLink)
		{
			break;
		}

		pList = pList->next;
	}

    return iPos;
}

/*
 * 获取节点索引
*/
int GetListIndex(List *pList, gDataPointer data)
{
    int iIndex = -1;

	while(pList != NULL)
	{
		++iIndex;
		if (pList->data == data)
		{
			break;
		}

		pList = pList->next;
	}

	return iIndex;
}

/*
 * 获取链表长度
*/
int GetListLength(List *pList)
{
    int iLength = 0;

	while(pList != NULL)
	{
		iLength++;

		pList = pList->next;
	}

    return iLength;
}