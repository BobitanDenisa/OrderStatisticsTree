/* Bobitan Denisa-Ioana

   Task:

   Implement correctly and efficiently the management operations of an order statistics tree.

   Observations:

   The algorithm has complexity O(n*h), where h is the height.

   Select: O(h)=O(logn)
   BuildHeap: O(n)

   Number of operations, both for the select and the delete have similar complexities, representing O(nlogn).

*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#pragma warning(disable : 4996)

Profiler profiler("demo");

int opSel, opDel;

typedef struct nod
{
	int id;
	struct nod *left;
	struct nod *right;
	struct nod *parent;
	int size;
} node;

node *newNode(int id)
{
	node *p = (node*)malloc(sizeof(node));
	p->id = id;
	p->left = p->right = p->parent = NULL;
	p->size = 1;
	return p;
}

node *buildTree(int start, int end, node *parent)
{
	if (start <= end)
	{
		int i = (start + end) / 2;
		node *root = newNode(i+1);
		root->parent = parent;
		root->left = buildTree(start, i - 1, root);
		root->right = buildTree(i + 1, end, root);
		//set size of each node
		if (root->left != NULL)
		{
			root->size += root->left->size;
		}
		if (root->right != NULL)
		{
			root->size += root->right->size;
		}
		return root;
	}
	else
	{
		return NULL;
	}
}

void prettyPrint(node *root, int level) //inorder
{
	if (root != NULL)
	{
		prettyPrint(root->right, level + 1);
		printf("\n");
		for (int i = 0; i <= level; i++)
		{
			printf("          ");
		}
		printf("%d,%d\n", root->id, root->size);
		prettyPrint(root->left, level + 1);
	}
}

void printTree(node *root)
{
	if (root == NULL)
	{
		printf("Empty tree!\n");
	}
	else
	{
		prettyPrint(root, 0);
	}
	printf("\n");
}

node *OS_select(node *root, int k)
{
	int size;
	if (root->left == NULL)
	{
		size = 1;
		opSel++;
	}
	else
	{
		size = root->left->size + 1;
		opSel++;
	}
	opSel++;
	if (k == size)
	{			
		return root;
	}
	else
	{
		opSel++;
		if (k < size)
		{
			return OS_select(root->left, k);
		}
		else
		{
			return OS_select(root->right, k - size);
		}
	}
}

node *findMin(node *root)
{
	while (root->left != NULL)
	{
		root = root->left;
	}
	return root;
}

node *findSuccessor(node *root)
{
	if (root->right != NULL)
	{
		return findMin(root->right);
	}
	node *aux = root->parent;
	while (aux != NULL && root == aux->right)
	{
		root = aux;
		aux = root->parent;
	}
	return aux;
}

void updateSize(node *n)
{
	node *aux = n->parent;
	while (aux != NULL)
	{
		aux->size--;
		opDel++;
		aux = aux->parent;
	}
}

node *OS_delete(node **root, node *z) //z is the node to delete
{
	node *y, *x;
	if (z->left == NULL || z->right == NULL)
	{
		y = z; //y is the node physically deleted
	}
	else
	{
		y = findSuccessor(z);
	}
	if (y->left != NULL)
	{
		x = y->left; //node x is y's child
	}
	else
	{
		x = y->right;
	}
	if (x != NULL)
	{
		x->parent = y->parent;
	}
	if (y->parent == NULL)
	{
		*root = x; //y's child becomes the new root
	}
	else
	{
		if (y == y->parent->left != NULL)
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}
	}
	updateSize(y);
	return y;
}

void forDemo(int n, node *root)
{
	node *p = OS_select(root, n);
	printf("Delete %d from pos %d:\n", p->id, n);
	node *nod = OS_delete(&root, p);
	p->id = nod->id;
	free(nod);
	printTree(root);
}
	
void demo()
{
	int n;
	node *root = (node*)malloc(sizeof(node));

	printf("Enter number of nodes in bst: ");
	scanf("%d", &n);
	root = buildTree(0, n-1, NULL);
	prettyPrint(root, 0);
	printf("\n");
	int r;
	printf("\n");

	/*
	//demo deletes all nodes of the tree one by one, in random order
	for (int i = 0; i < n; i++)
	{
		r = rand() % root->size + 1;
		node *p = OS_select(root, r);
		printf("Delete %d from pos %d:\n", p->id, r);
		node *nod = OS_delete(&root, p);
		p->id = nod->id;
		free(nod);
		printTree(root);
	}
	*/

	//demo deletes a leaf, a node with only one child and a node with more children
	//used for a tree with 11 nodes: delete 9, 1 and 5
	int noua, unu, cinci;

	//9 is a node with more than 1 children
	noua = 9; //9 is on the 9th position 
	forDemo(noua, root);

	//1 is a node with one child
	unu = 1; //1 is on the 1st position after the deletion
	forDemo(unu, root);

	//5 is a leaf
	cinci = 4; //5 is now the 4th element in the tree
	forDemo(cinci, root);
}

void report()
{
	node *root = (node*)malloc(sizeof(node));
	int r;
	node *p;
	for (int i = 100; i <  10000; i += 100)
	{
		opDel = opSel = 0;
		for (int j = 0; j < 5; j++)
		{
			root = buildTree(0, i - 1, NULL);
			while (root != NULL)
			{
				r = rand() % root->size + 1;
				p = OS_select(root, r);
				printf("length %d, run %d: delete %d from %d\n", i, j, p->id, r);
				node *nod = OS_delete(&root, p);
				p->id = nod->id;
				free(nod);
				// printTree(root);
			}
		}
		profiler.countOperation("OperationsSelect", i, opSel / 5);
		profiler.countOperation("OperationsDelete", i, opDel / 5);
	}
	profiler.showReport();
}

int main()
{
	int x = 1;
	while (x != 0)
	{
		printf("Enter:\n 0 to EXIT \n 1 to TEST \n 2 to create the report \n");
		scanf("%d", &x);
		printf("\n");
		if (x == 1)
		{
			demo();
		}
		if (x == 2)
		{
			report();
		}
	}
	return 0;
}