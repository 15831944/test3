#ifndef __TREETEST_H__
#define __TREETEST_H__

#include "NodeTree.h"
typedef struct tagNodeData{
	unsigned int	nID;
	char			szLabel[128];
	bool			bOpen;
}NodeData, *pNodeData;

class TREETest
{
public:
	TREETest();
	~TREETest();
	
public:	
	TreeNode* 			CreateRootNode();
	TreeNode*			InsertChildNode(TreeNode *pParentNode, const char* szLabel, int nID);
	TreeNode* 			InsertSiblingNode(TreeNode *pAfterNode, const char* szLabel, int nID);
	
	void				DeleteNode(TreeNode *pNode);
	
};
#endif