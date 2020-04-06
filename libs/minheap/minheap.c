#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minheap.h"
#include "./../bintree/bintree.h"

/**
 * Create a HeapNode and return the pointer to it.
 * 
 * Helps make codebase more readable
 */
HeapNode *createHeapNode(char *data, int freq)
{
  HeapNode *t = malloc(sizeof(HeapNode));
  t->isStoringChar = 1;
  t->data = data;
  t->freq = freq;
  return t;
}

HeapNode *createHeapNode_TreeNode(BinTreeNode *treeNode, char *data, int freq)
{
  HeapNode *t = malloc(sizeof(HeapNode));
  t->isStoringChar = 0;
  t->treeNode = treeNode;
  t->data = data;
  t->freq = freq;
  return t;
}

/**
 * Create a MinHeap and return the pointer to it.
 * 
 * Helps make codebase more readable
 */
MinHeap *createMinHeap()
{
  MinHeap *t = malloc(sizeof(MinHeap));
  t->size = 0;
  return t;
}

/**
 * Insert a HeapNode into an already existing minHeap
 */
void minHeap_insert(MinHeap *minHeap, HeapNode *heapNode)
{
  //We are keeping the MinHeap size dynamically allocated, so allocate space for the new item
  if (minHeap->size == 0)
  {
    minHeap->elements = malloc(sizeof(HeapNode *));
  }
  else
  {
    minHeap->elements = realloc(minHeap->elements, (minHeap->size + 1) * sizeof(HeapNode *));
  }

  minHeap->size++;
  int i = minHeap->size - 1;
  minHeap->elements[i] = heapNode;
  while (i != 0 && heapNode->freq <= minHeap->elements[PARENT(i)]->freq)
  {
    HeapNode *t = minHeap->elements[i];
    minHeap->elements[i] = minHeap->elements[PARENT(i)];
    minHeap->elements[PARENT(i)] = t;
    i = PARENT(i);
  }

  minHeap->elements[i] = heapNode;
}

/**
 * Search for a HeapNode if it exists. If it does not, then return NULL
 */
HeapNode *minHeap_search(MinHeap *minHeap, char *data)
{
  int i = 0;
  for (i = 0; i < minHeap->size; i++)
  {
    if (strcmp(minHeap->elements[i]->data, data) == 0)
    {
      return minHeap->elements[i];
    }
  }

  return NULL;
}

/**
 * Perform heapify algorithm to ensure the first element is the smallest element in the entire heap
 */
void minHeap_heapify(MinHeap *minHeap, int i)
{
  int smallest = i;
  int l = LCHILD(i);
  int r = RCHILD(i);

  if (l < minHeap->size && minHeap->elements[l]->freq < minHeap->elements[i]->freq)
  {
    smallest = l;
  }

  if (r < minHeap->size && minHeap->elements[r]->freq < minHeap->elements[smallest]->freq)
  {
    smallest = r;
  }

  if (smallest != i)
  {
    HeapNode *t = minHeap->elements[0];
    minHeap->elements[0] = minHeap->elements[smallest];
    minHeap->elements[smallest] = t;
    minHeap_heapify(minHeap, smallest);
  }
}

void minHeap_heapsort(MinHeap *minHeap, int n)
{
  int i;
  for (i = minHeap->size - 1; i >= 0; i--)
  {
    HeapNode *t = minHeap->elements[0];
    minHeap->elements[0] = minHeap->elements[i];
    minHeap->elements[i] = t;
    minHeap_heapify(minHeap, 0);
  }

  int smallest = minHeap->elements[0]->freq;
  int smallestIdx = 0;
  i = 0;
  for (i; i < minHeap->size; i++)
  {
    if (minHeap->elements[i]->freq < smallest)
    {
      smallestIdx = i;
    }
  }

  HeapNode *t = minHeap->elements[smallestIdx];
  minHeap->elements[smallestIdx] = minHeap->elements[0];
  minHeap->elements[0] = t;
  return;
}

/**
 * Remove the smallest element from a MinHeap and heapify the heap to maintain property
 */
HeapNode *minHeap_delete(MinHeap *minHeap)
{
  if (minHeap->size > 0)
  {
    HeapNode *t = minHeap->elements[0];
    //Swap the last element into the first element spot, and shrink the size of the minHeap
    minHeap->elements[0] = minHeap->elements[minHeap->size - 1];
    minHeap->size = minHeap->size - 1;
    //Reallocate heap to be 1 element smaller
    minHeap->elements = realloc(minHeap->elements, minHeap->size * sizeof(HeapNode *));
    minHeap_heapify(minHeap, 0);
    return t;
  }

  return NULL;
}

void printMinHeap(MinHeap *minHeap)
{
  if (minHeap == NULL)
    return;
  int i;
  for (i = 0; i < minHeap->size; i++)
  {
    printf("{'%s', %d}\n", minHeap->elements[i]->data, minHeap->elements[i]->freq);
  }

  printf("\n");
}
