#ifndef __NODETREE_H__
#define __NODETREE_H__

typedef void* ElementType;
typedef struct tagTreeNode{
	ElementType data;
	tagTreeNode	*prev;
	tagTreeNode	*next;
	tagTreeNode	*parent;
	tagTreeNode	*children;
}TreeNode, *pTreeNode;


TreeNode* 		g_node_new(void	*data);
void			g_node_destroy(TreeNode *root);
void			g_nodes_free(TreeNode *node);
void			g_node_unlink(TreeNode *node);

int				g_node_child_index(TreeNode *node, void *data);
int 			g_node_child_position(TreeNode *node, TreeNode *child);
unsigned int	g_node_n_children(TreeNode *node);
unsigned int	g_node_depth(TreeNode *node);
unsigned int	g_node_max_height(TreeNode *root);

TreeNode*		g_node_nth_child(TreeNode *node, unsigned int n);
TreeNode*		g_node_last_child(TreeNode *node);
TreeNode*		g_node_first_sibling(TreeNode *node);
TreeNode*		g_node_last_sibling(TreeNode *node);

TreeNode*		g_node_insert_before(TreeNode *parent, TreeNode *sibling, TreeNode *node);
TreeNode*		g_node_insert_after(TreeNode *parent, TreeNode *sibling, TreeNode *node);
TreeNode*		g_node_insert(TreeNode *parent, int position, TreeNode *node);
TreeNode*		g_node_prepend(TreeNode *parent, TreeNode *node);

//TreeNode*		g_node_copy_deep(TreeNode *node, GCopyFunc  copy_func, void *data);	
TreeNode*		g_node_copy(TreeNode *node);
TreeNode*		g_node_get_root(TreeNode *node);

bool			g_node_is_ancestor(TreeNode *node, TreeNode *descendant);
void			g_node_reverse_children(TreeNode *node);

#define	G_NODE_IS_ROOT(node)								(((TreeNode*)(node))->parent == NULL && ((TreeNode*)(node))->prev == NULL && ((TreeNode*)(node))->next == NULL)

#define	G_NODE_IS_LEAF(node)								(((TreeNode*)(node))->children == NULL)

#define	g_node_append(parent, node)							g_node_insert_before ((parent), NULL, (node))

#define	g_node_insert_data(parent, position, data)			g_node_insert((parent), (position), g_node_new (data))

#define g_node_insert_data_after(parent, sibling, data)		g_node_insert_after((parent), (sibling), g_node_new (data))

#define	g_node_insert_data_before(parent, sibling, data)	g_node_insert_before((parent), (sibling), g_node_new (data))

#define g_node_prepend_data(parent, data)					g_node_prepend((parent), g_node_new (data))

#define	g_node_append_data(parent, data)					g_node_insert_before((parent), NULL, g_node_new (data))

#define g_node_prev_sibling(node)							((node) ? ((TreeNode*)(node))->prev : NULL)

#define g_node_next_sibling(node)							((node) ? ((TreeNode*)(node))->next : NULL)

#define	g_node_first_child(node)							((node) ? ((TreeNode*)(node))->children : NULL)
				
#endif