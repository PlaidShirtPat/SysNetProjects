#include "priorityQueue.h"

PriorityQueue *newQueue(int size)
{
	PriorityQueue *queue = malloc(sizeof(PriorityQueue));
	queue->queue = malloc(sizeof(Pair)*size);
	queue->size = 0;
	queue->maxSize = size;
	return queue;

}

void freeQueue(PriorityQueue *queue)
{
	free(queue->queue);
	free(queue);
}

//inserts at end and sorts
//will return false if insert fails due to max size being reached
bool push(PriorityQueue *queue, Pair *value)
{
	if(queue->size < queue->maxSize)
	{
		queue->queue[queue->size] = value;
		queue->size++;
		//we didn't actually need a priority queue. whoops
		//sortQueue(queue);
		return true;
	}
	else
		return false;
}

//returns null on error
Pair *pop(PriorityQueue *queue)
{
	if(queue->size > 0)
	{
		Pair *pop = queue->queue[0];
		queue->size--;
		memmove(&(queue->queue[0]), &(queue->queue[1]), queue->size*sizeof(Pair*));
		return pop;
	}
	else
		return NULL;
}

void sortQueue(PriorityQueue *queue)
{
	bubbleSortQueue(queue);
}

//we're just going to use a bubble sort because it's simple and our lists are not going to be big.
void bubbleSortQueue(PriorityQueue *queue)
{
	int i=0,j=0;
	for(;i<queue->size-1;i++)
	{
		for(j=i;j<queue->size-1;j++)
		{
			if(queue->queue[j]->value > queue->queue[j+1]->value)
				swap(queue, j, j+1);
		}
	}
}

void swap(PriorityQueue *queue, int index1, int index2)
{
	Pair *temp = queue->queue[index1];
	queue->queue[index1] = queue->queue[index2];
	queue->queue[index2] = temp;
}

//buffer is expected to be 5 times the size of maxsize of queue
void toString(PriorityQueue *queue, char *buffer)
{
	int i;
	char *buffPt = buffer;
	for(i=0 ;i < queue->size; i++)
	{
		sprintf(buffPt, "%c:%d,", queue->queue[i]->label,queue->queue[i]->value);
		buffPt = &buffPt[strlen(buffPt)];
	}
}
