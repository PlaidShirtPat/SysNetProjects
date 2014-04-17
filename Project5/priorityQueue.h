//When this was originally created, we thought we needed a priority queue. part way through implimenting, we releaized we didn't need it to be priority, just a normal queue. To make this change the sort on insert is commented out.
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include "MyNetworking.h"

typedef struct{
	char label;
	int value;
} Pair;

typedef struct{
	Pair **queue;
	int size;
	int maxSize;
} PriorityQueue; 

PriorityQueue *newQueue(int size);
void freeQueue(PriorityQueue *queue);
bool push(PriorityQueue *queue, Pair *value);
Pair *pop(PriorityQueue *queue);
void sortQueue(PriorityQueue *queue);
void bubbleSortQueue(PriorityQueue *queue);
void swap(PriorityQueue *queue, int index1, int index2);
void toString(PriorityQueue *queue, char *buffer);
#endif //PRIORITY_QUEUE_H
