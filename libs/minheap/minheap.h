/*
Header file for Minheap
*/

//Prevent's double include
#ifndef MINHEAP_H
#define MINHEAP_H

#include "./../bintree/bintree.h"

//Define operators to make it easier to work with the heap
#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2

//HeapNode Struct Definition
typedef struct HeapNode
{
  int isStoringChar;
  BinTreeNode* treeNode;
  char *data;
  int freq;
} HeapNode;

//Heap Struct Definition
typedef struct MinHeap
{
  int size;
  HeapNode** elements;
} MinHeap;

//Function Prototype for creating a HeapNode
HeapNode* createHeapNode(char* data, int freq);

//Function Prototype for creating a HeapNode that is referencing a treeNode
HeapNode* createHeapNode_TreeNode(BinTreeNode* treeNode, char* data, int freq);

//Function Prototype for creating a MinHeap
MinHeap* createMinHeap();

//Function Prototype for inserting a HeapNode into an existing MinHeap
void minHeap_insert(MinHeap* minHeap, HeapNode* heapNode);

//Function Prototype for deleting the min HeapNode in a MinHeap
HeapNode* minHeap_delete(MinHeap* minHeap);

//Function Prototype for searching for a node in a Minheap
HeapNode* minHeap_search(MinHeap* minHeap, char* data);

//Function Prototype for heapify a MinHeap
void minHeap_heapify(MinHeap* minHeap, int idx);

//Function Prototype for heapsort
void minHeap_heapsort(MinHeap* minHeap, int);

//Function Prototype for BFS printing (level by level)
void printMinHeap(MinHeap* minHeap);

#endif