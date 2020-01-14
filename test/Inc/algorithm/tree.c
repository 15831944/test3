#include "StdAfx.h"
#include "NodeTree.h"


TreeNode* g_node_new(void	*data)
{
	TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
	if(node == NULL)
	{
		return NULL;
	}
	memset(node, 0x0, sizeof(TreeNode));
	node->data = data;
	node->prev = NULL;
	node->next = NULL;
	node->parent = NULL;
	node->children = NULL;
	
	return node;
}

void g_node_destroy(TreeNode *root)
{
	if(!G_NODE_IS_ROOT(root))
		g_node_unlink(root);
	
	g_nodes_free(root);
}

void g_nodes_free(TreeNode *node)
{
	while(node)
	{
		TreeNode *next = node->next;
		if(node->children)
			g_nodes_free(node->children);
		
		free(node);
		node = NULL;
		
		node = next;
	}
}

void g_node_unlink(TreeNode *node)
{
	if(node->prev)
	{
		node->prev->next = node->next;
	}
	else if(node->parent)
	{
		node->parent->children = node->next;
	}
	
	node->parent = NULL;
	if(node->next)
	{
		node->next->prev = node->prev;
		node->next = NULL;
	}
	node->prev = NULL;
}

//返回数据data在node中的索引号
int g_node_child_index(TreeNode *node, void *data)
{
	unsigned int n = 0;
	
	node = node->children;
	while(node)
	{
		if(node->data == data)
			return n;
		n++;
		node = node->next;
	}
	
	return -1;
}

//返回子节点child在node的位置
int g_node_child_position(TreeNode *node, TreeNode *child)
{
	unsigned int n = 0;
	
	node = node->children;
	while(node)
	{
		if(node == child)
			return n;

		n++;
		node = node->next;
	}
	
	return -1;
}

//返回节点node的子节点个数
unsigned int g_node_n_children(TreeNode *node)
{
	unsigned int n = 0;
	
	node = node->children;
	while(node)
	{
		n++;
		node = node->next;
	}
	
	return n;
}

//返回节点node在树中的深度
unsigned int g_node_depth(TreeNode *node)
{
	unsigned int depth = 0;
	
	while(node)
	{
		depth++;
		node = node->parent;
	}
	
	return depth;
}

//返回树的高度
unsigned int g_node_max_height(TreeNode *root)
{
	TreeNode *child = NULL;
	unsigned int max_height = 0;
	
	if(!root)
		return 0;
	
	child = root->children;
	while(child)
	{
		unsigned int uheight = 0;

		uheight = g_node_max_height(child);
		if (uheight > max_height)
			max_height = uheight;
		child = child->next;
	}
	
	return max_height + 1;
}

//返回节点node的第N个子节点
TreeNode* g_node_nth_child(TreeNode *node, unsigned int n)
{
	node = node->children;
	if(node)
	{
		while ((n-- > 0) && node)
			node = node->next;
	}
	
	return node;
}

//返回节点node的最后一个子节点
TreeNode* g_node_last_child(TreeNode *node)
{
	node = node->children;
	if(node)
	{
		while(node->next)
			node = node->next;
	}
	
	return node;
}

//返回节点node的子节点第一个兄弟节点
TreeNode* g_node_first_sibling(TreeNode *node)
{
	if(node->parent)
		return node->parent->children;

	while(node->prev)
		node = node->prev;
	
	return node;
}

//返回节点node的子节点最后一个兄弟节点
TreeNode* g_node_last_sibling(TreeNode *node)
{
	while(node->next)
		node = node->next;

	return node;
}

//在父节点parent中的节点sibling之前插入节点node
TreeNode* g_node_insert_before(TreeNode *parent, TreeNode *sibling, TreeNode *node)
{
	node->parent = parent;
	
	if(sibling)
	{
		if(sibling->prev)
		{
			node->prev = sibling->prev;
			node->prev->next = node;
			node->next = sibling;
			sibling->prev = node;
		}
		else
		{
			node->parent->children = node;
			node->next = sibling;
			sibling->prev = node;
		}
	}
	else
	{
		if(parent->children)
		{
			sibling = parent->children;
			while(sibling->next)
				sibling = sibling->next;
			
			node->prev = sibling;
			sibling->next = node;
		}
		else
		{
			node->parent->children = node;
		}
	}
	
	return node;
}

//
TreeNode* g_node_insert_after(TreeNode *parent, TreeNode *sibling, TreeNode *node)
{
	node->parent = parent;
	
	if(sibling)
	{
		if(sibling->next)
		{
			sibling->next->prev = node;
		}
		
		node->next = sibling->next;
		node->prev = sibling;
		sibling->next = node;
	}
	else
	{
		if(parent->children)
		{
			node->next = parent->children;
			parent->children->prev = node;
		}
		else
		{
			parent->children = node;
		}
	}
	
	return node;
}

TreeNode* g_node_insert(TreeNode *parent, int position, TreeNode *node)
{
	if(position > 0)
	{
		return g_node_insert_before(parent, g_node_nth_child(parent, position), node);
	}
	else if(position == 0)
	{
		return g_node_prepend(parent, node);
	}
	else
	{
		return g_node_append(parent, node);
	}
}

TreeNode* g_node_prepend(TreeNode *parent, TreeNode *node)
{
	return g_node_insert_before(parent, parent->children, node);
}

TreeNode* g_node_copy(TreeNode *node)
{
	TreeNode *new_node = NULL;
	
	if(node)
	{
		TreeNode *child;
		
		new_node = g_node_new(node->data);
		
		for(child = g_node_last_child(node); child; child = child->prev)
		{
			g_node_prepend(new_node, g_node_copy(child));
		}
	}
	
	return new_node;
}

TreeNode* g_node_get_root(TreeNode *node)
{
	while(node->parent)
	{
		node = node->parent;
	}
	
	return node;
}

bool g_node_is_ancestor(TreeNode *node, TreeNode *descendant)
{
	while(descendant)
	{
		if(descendant->parent == node)
		{
			return true;
		}
		
		descendant = descendant->parent;
	}
	
	return false;
}

void g_node_reverse_children(TreeNode *node)
{
	TreeNode *child = NULL;
	TreeNode *last  = NULL;

	child = node->children;
	last  = NULL;

	while(child)
	{
		last = child;
		child = last->next;
		last->next = last->prev;
		last->prev = child;
	}

	node->children = last;
}