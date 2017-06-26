#include "stdafx.h"
#include "TREETest.h"

TREETest::TREETest()
{
}

TREETest::~TREETest()
{
}

TreeNode* TREETest::CreateRootNode()
{
	TreeNode *pRoot = NULL;
	NodeData *pData = NULL;

	pData = new NodeData;
	if (pData == NULL)
	{
		return NULL;
	}
	memset(pData, 0x0, sizeof(NodeData));

	pData->nID = 0;
	strcpy_s(pData->szLabel, _T("¸ù½Úµã:"));
	pData->bOpen = true;

	pRoot = g_node_new(pData);
	return pRoot;
}

TreeNode* TREETest::InsertChildNode(TreeNode *pParentNode, const char* szLabel, int nID)
{
	TreeNode *newNode = NULL;
	NodeData *pData	  = NULL;

	pData = new NodeData;
	if (pData == NULL)
	{
		return NULL;
	}
	memset(pData, 0x0, sizeof(NodeData));

	pData->nID = nID;
	strcpy_s(pData->szLabel, szLabel);
	pData->bOpen = true;

	newNode = g_node_append(pParentNode, g_node_new(pData));
	return newNode;
}

TreeNode* TREETest::InsertSiblingNode(TreeNode *pAfterNode, const char* szLabel, int nID)
{
	TreeNode *newNode = NULL;
	NodeData *pData	  = NULL;

	pData = new NodeData;
	if (pData == NULL)
	{
		return NULL;
	}
	memset(pData, 0x0, sizeof(NodeData));

	pData->nID = nID;
	strcpy_s(pData->szLabel, szLabel);
	pData->bOpen = true;

	newNode = g_node_insert_data_after(pAfterNode->parent, pAfterNode, pData);
	return newNode;
}

void TREETest::DeleteNode(TreeNode *pNode)
{
	if (pNode == NULL)
	{
		return;
	}

	g_node_destroy(pNode);
}