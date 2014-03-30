
typedef struct{


} PriortyQueue;




typdef struct{
	char label;
	int value;
	//a is 'right' and greater than b
	HeapNode *a;
	HeapNode *b;
} HeapNode;

typedef struct{
	HeapNode *head;
} Heap;


HeapNode *createNode(char label, int value, HeapNode *childA, HeapNode *childB);
void freeHeap(Heap *heap);
void freeNodeRecursive(HeapNode *node);
