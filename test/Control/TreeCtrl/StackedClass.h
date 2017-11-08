#ifndef __STACKEDCLASS_H__
#define __STACKEDCLASS_H__

typedef enum tagSC_TreeItem{
	SC_EMPTY_TYPE = 0,
	SC_ROOT_TYPE,
	SC_FIRST_TYPE,
	SC_LAST_TYPE,
	SC_SORE_TYPE
}SC_TREEITEM;

typedef struct tagSC_InsertStruct{
	UINT			nState;
	LPSTR			pszText;
	int				nTextMax;
	int				nImage;
	int				nSelectedImage;
	CWnd*			pCurWndItem;
	LPARAM			lParam; 
}SC_INSERTSTRUCT;

typedef struct tagSC_Node{
	tagSC_Node*		prevNode;
	tagSC_Node*		nextNode;
	tagSC_Node*		parentNode;
	tagSC_Node*		childrenNode;
	SC_TREEITEM		enTreeItem;
	SC_INSERTSTRUCT	hInsertStruct;
}SC_NODE;

class CStackedClass
{
public:
	CStackedClass();
	~CStackedClass();
	
public:
	BOOL					AddPane(SC_TREEITEM enTreeItem, SC_INSERTSTRUCT* pInsertStruct);
	BOOL					ShowPane();

protected:
	SC_NODE*				CreateNewNode(void* pData, unsigned int nDataLen);
	void					DestroyRootNode(SC_NODE* pRootNode);

	unsigned int			GetNodeDepth(SC_NODE* pNode);

	SC_NODE*				InsertBeforeNode(SC_NODE* pParentNode, SC_NODE* pSiblingNode, SC_NODE* pNode);
	SC_NODE*				InsertAfterNode(SC_NODE* pParentNode, SC_NODE* pSiblingNode, SC_NODE* pNode);


private:
	SC_NODE*				m_pRootNode;
	std::vector<SC_NODE*>	m_vecNode;
};

#endif