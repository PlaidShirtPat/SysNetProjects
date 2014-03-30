#include "minheap.h"

HeapNode *createNode(char label, int value, HeapNode *childA, HeapNode *childB)
{
	HeapNode *node = malloc(sizeof(HeapNode));
	node->label = label;
	node->value = value;
	node->a = childA;
	node->b = childB;
	
	return node;
}

void freeHeap(Heap *heap)
{
	freeNodeRecursive(heap->head);
	free(heap);
}

void freeNodeRecursive(HeapNode *node)
{
	freeNodeRecursive(node->a);
	freeNodeRecursive(node->b);
	free(node);
}

Heap *createHeap(RoutingTable *table)
{
	Heap *heap = malloc(sizeof(Heap));
	heap->head = NULL;
	int i;
	for(i=0; i<table->numNodes; i++)
		heapInsert(heap, createNode(table->labels[i], table->lengths[i], NULL, NULL));

	return heap;
}

void heapInsert(Heap *heap, HeapNode *node)
{
	if(heap->head == NULL)
		heap->head = node;
	else
	{
		//check to see if we are pivoting the head, if so replace the head
		if(heap->head->value < node->value)
		{
			piviotNode(heap->head, node);
			heap->head = node;
		}

		heap->head = heapInsertRecurse(heap->head, node);
	}

	return;
}

void pivotNode(HeapNode *old, HeapNode *new)
{
	//if there is no A child node
	if(new->a == NULL)
	{
		new->a = old;
		if(new->b == NULL)
		{
		new->b = old->b;
		if(old->a != NULL)
			 old->b = old->a->b;
		}
	}

}

//returns the current head of the heap
HeapNode *heapInsertRecurse(HeapNode *currNode, HeapNode *insertNode)
{
	//option one, we replace the value & rotate
	if(currNode->value < insertNode->value)
		pivotNode(currNode, insertNode);
	//option one
}


