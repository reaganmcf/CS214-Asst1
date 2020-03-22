/*
Header file for Minheap
*/

//Prevent's double include
#ifndef MINHEAP_H
#define MINHEAP_H

//Define operators to make it easier to work with the heap
#define LCHILD(x) 2 * x + 1;
#define RCHILD(x) 2 * x + 2;
#define PARENT(x) (x - 1) / 2;

//HeapNode Struct Definition
typedef struct HeapNode
{
  char *data;
  int freq;
} HeapNode;

//Heap Struct Definition
typedef struct MinHeap
{
  int size;
  HeapNode *head;
} MinHeap;

#endif