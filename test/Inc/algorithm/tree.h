#ifndef __TREE_H__
#define __TREE_H__

typedef void* gDataPointer;
typedef struct TreeNode
{
	gDataPointer data;
	struct TreeNode	*prev;
	struct TreeNode	*next;
	struct TreeNode	*parent;
	struct TreeNode	*children;
}Tree, *pTree;

Tree* CreateTree(gDataPointer data);
void			g_node_destroy(Tree *root);
void			g_nodes_free(Tree *node);
void			g_node_unlink(Tree *node);

int				g_node_child_index(Tree *node, void *data);
int 			g_node_child_position(Tree *node, Tree *child);
unsigned int	g_node_n_children(Tree *node);
unsigned int	g_node_depth(Tree *node);
unsigned int	g_node_max_height(Tree *root);

Tree*		g_node_nth_child(Tree *node, unsigned int n);
Tree*		g_node_last_child(Tree *node);
Tree*		g_node_first_sibling(Tree *node);
Tree*		g_node_last_sibling(Tree *node);

Tree*		g_node_insert_before(Tree *parent, Tree *sibling, Tree *node);
Tree*		g_node_insert_after(Tree *parent, Tree *sibling, Tree *node);
Tree*		g_node_insert(Tree *parent, int position, Tree *node);
Tree*		g_node_prepend(Tree *parent, Tree *node);

//Tree*		g_node_copy_deep(Tree *node, GCopyFunc  copy_func, void *data);	
Tree*		g_node_copy(Tree *node);
Tree*		g_node_get_root(Tree *node);

bool			g_node_is_ancestor(Tree *node, Tree *descendant);
void			g_node_reverse_children(Tree *node);

#define	G_NODE_IS_ROOT(node)								(((Tree*)(node))->parent == NULL && ((Tree*)(node))->prev == NULL && ((Tree*)(node))->next == NULL)

#define	G_NODE_IS_LEAF(node)								(((Tree*)(node))->children == NULL)

#define	g_node_append(parent, node)							g_node_insert_before ((parent), NULL, (node))

#define	g_node_insert_data(parent, position, data)			g_node_insert((parent), (position), g_node_new (data))

#define g_node_insert_data_after(parent, sibling, data)		g_node_insert_after((parent), (sibling), g_node_new (data))

#define	g_node_insert_data_before(parent, sibling, data)	g_node_insert_before((parent), (sibling), g_node_new (data))

#define g_node_prepend_data(parent, data)					g_node_prepend((parent), g_node_new (data))

#define	g_node_append_data(parent, data)					g_node_insert_before((parent), NULL, g_node_new (data))

#define g_node_prev_sibling(node)							((node) ? ((Tree*)(node))->prev : NULL)

#define g_node_next_sibling(node)							((node) ? ((Tree*)(node))->next : NULL)

#define	g_node_first_child(node)							((node) ? ((Tree*)(node))->children : NULL)
				
#endif