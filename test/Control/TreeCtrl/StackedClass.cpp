#include "stdafx.h"
#include "StackedClass.h"

CStackedClass::CStackedClass()
{
	m_pRootNode = NULL;
}

CStackedClass::~CStackedClass()
{
}

BOOL CStackedClass::AddPane(SC_TREEITEM enTreeItem, SC_INSERTSTRUCT* pInsertStruct)
{
	int nStructLen = 0;
	SC_NODE* pNode = NULL;

	if (enTreeItem == SC_EMPTY_TYPE)
	{
		return FALSE;
	}

	if (pInsertStruct == NULL)
	{
		return FALSE;
	}

	nStructLen = sizeof(SC_INSERTSTRUCT);
	pNode = CreateNewNode(pInsertStruct, nStructLen);
	if (pNode == NULL)
	{
		return FALSE;
	}

	if (enTreeItem == SC_ROOT_TYPE)
	{
		pNode->enTreeItem = SC_ROOT_TYPE;
		m_pRootNode = pNode;
	}
	else
	{

	}

	return TRUE;
}

BOOL CStackedClass::ShowPane()
{
	return TRUE;
}

SC_NODE* CStackedClass::CreateNewNode(void* pData, unsigned int nDataLen)
{
	if (pData == NULL || nDataLen == 0)
	{
		return NULL;
	}

	SC_NODE* pNode = new SC_NODE;
	if (pNode == NULL)
	{
		return NULL;
	}
	memset(pNode, 0x0, sizeof(SC_NODE));

	pNode->prevNode = NULL;
	pNode->nextNode = NULL;
	pNode->parentNode = NULL;
	pNode->childrenNode = NULL;
	memcpy(&pNode->hInsertStruct, pData, nDataLen);

	return pNode;
}

void CStackedClass::DestroyRootNode(SC_NODE* pRootNode)
{
	if (pRootNode == NULL)
	{
		return;
	}
}

unsigned int CStackedClass::GetNodeDepth(SC_NODE* pNode)
{
	unsigned int nDepth = 0;

	while(pNode != NULL)
	{
		nDepth++;
		pNode = pNode->nextNode;
	}

	return nDepth;
}

SC_NODE* CStackedClass::InsertBeforeNode(SC_NODE* pParentNode, SC_NODE* pSiblingNode, SC_NODE* pNode)
{
	pNode->parentNode = pParentNode;

	if (pSiblingNode != NULL)
	{
		if (pSiblingNode->prevNode != NULL)
		{
			pNode->prevNode = pSiblingNode->prevNode;
			pNode->prevNode->nextNode = pNode;
			pNode->nextNode = pSiblingNode;
			pSiblingNode->prevNode = pNode;
		}
		else
		{
			pNode->parentNode->childrenNode = pNode;
			pNode->nextNode = pSiblingNode;
			pSiblingNode->prevNode = pNode;
		}
	}
	else
	{
		if (pParentNode->childrenNode != NULL)
		{
			pSiblingNode = pParentNode->childrenNode;
			while(pSiblingNode->nextNode)
			{
				pSiblingNode = pSiblingNode->nextNode;
			}

			pNode->prevNode = pSiblingNode;
			pSiblingNode->nextNode = pNode;
		}
		else
		{
			pNode->parentNode->childrenNode = pNode;
		}
	}

	return pNode;
}

SC_NODE* CStackedClass::InsertAfterNode(SC_NODE* pParentNode, SC_NODE* pSiblingNode, SC_NODE* pNode)
{
	pNode->parentNode = pParentNode;

	if (pSiblingNode != NULL)
	{
		if (pSiblingNode->nextNode != NULL)
		{
			pSiblingNode->nextNode->prevNode = pNode;
		}

		pNode->nextNode = pSiblingNode->nextNode;
		pNode->prevNode = pSiblingNode;
		pSiblingNode->nextNode = pNode;
	}
	else
	{
		if (pParentNode->childrenNode != NULL)
		{
			pNode->nextNode = pParentNode->childrenNode;
			pParentNode->childrenNode->prevNode = pNode;
		}
		else
		{
			pParentNode->childrenNode = pNode;
		}
	}

	return pNode;
}