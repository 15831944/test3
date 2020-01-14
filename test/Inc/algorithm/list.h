#ifndef __LIST_H__
#define __LIST_H__

typedef void* gDataPointer;
typedef struct listNode
{
    gDataPointer data;
    struct listNode *next;
}List, *pList;

//创建链表节点
List* CreateList(gDataPointer data);

//删除链表节点
void DeleteList(List *pList);

//插入链表位置数据
List* InsertList(List *pList, gDataPointer data, int pos);

//插入链表节点数据
List* InsertList_before(List *pList, List *pSibing, gDataPointer data);

//连接两个链表
List* ConCatList(List *pList1, List *pList2);

//移除链表数据
List* RemoveData(List *pList, gDataPointer data);

//移除链表节点
List* RemoveLink(List *pList, List *pLink);

//拷贝链表
List* CopyList(List *pList);

//翻转链表
List* ReverseList(List *pList);

//查找链表
List* FindList(List *pList);

//获取链表最后一个节点
List *GetLastList(List *pList);

//获取节点位置
int GetListPos(List *pList, List *pLink);

//获取节点索引
int GetListIndex(List *pList, gDataPointer data);

//获取链表长度
int GetListLength(List *pList);

#endif