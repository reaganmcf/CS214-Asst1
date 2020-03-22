#include<stdlib.h>
#include<stdio.h>
#include "minheap.h"

/**
 * Create a HeapNode and return the pointer to it.
 * 
 * Helps make codebase more readable
 */
HeapNode* createHeapNode(char* data, int freq) {
  HeapNode* t = malloc(sizeof(HeapNode));
  t->data = data;
  t->freq = freq;
  return t;
}

/**
 * Create a MinHeap and return the pointer to it.
 * 
 * Helps make codebase more readable
 */
MinHeap* createMinHeap() {
  MinHeap* t = malloc(sizeof(MinHeap));
  t->size = 0;
  return t;
}

/**
 * Insert a HeapNode into an already existing minHeap
 */
void minHeap_insert(MinHeap* minHeap, HeapNode* heapNode) {
  //We are keeping the MinHeap size dynamically allocated, so allocate space for the new item
  if(minHeap->size == 0) {
    minHeap->elements = malloc(sizeof(HeapNode*));
  } else {
    minHeap->elements = realloc(minHeap->elements, (minHeap->size + 1) * sizeof(HeapNode*));
  }

  int i = minHeap->size++;
  while(i != 0 && heapNode->freq < minHeap->elements[PARENT(i)]->freq) {
    minHeap->elements[i] = minHeap->elements[PARENT(i)];
    i = PARENT(i);
  }
  
  minHeap->elements[i] = heapNode;
}

void minHeap_swap(HeapNode** heapNode1, HeapNode** heapNode2) {
  HeapNode* temp = *heapNode1;
  *heapNode1 = *heapNode2;
  *heapNode2 = temp;
}

/**
 * Perform heapify algorithm to ensure the first element is the smallest element in the entire heap
 */
void minHeap_heapify(MinHeap* minHeap, int i) {
  int smallest = i;
  if(LCHILD(i) < minHeap->size && minHeap->elements[LCHILD(i)]->freq < minHeap->elements[i]->freq) {
    smallest = LCHILD(i);
  }
  if(RCHILD(i) < minHeap->size && minHeap->elements[RCHILD(i)]->freq < minHeap->elements[smallest]->freq) {
    smallest = RCHILD(i);
  }
  if(smallest != i) {
    minHeap_swap(&(minHeap->elements[i]), &(minHeap->elements[smallest]));
    minHeap_heapify(minHeap, smallest);
  }
}

/**
 * Remove the smallest element from a MinHeap and heapify the heap to maintain property
 */
HeapNode* minHeap_delete(MinHeap* minHeap) {
  if(minHeap->size > 0) {
    HeapNode* t = minHeap->elements[0];
    //Swap the last element into the first element spot, and shrink the size of the minHeap
    minHeap->elements[0] = minHeap->elements[minHeap->size - 1];
    minHeap->size = minHeap->size - 1;
    //Reallocate heap to be 1 element smaller
    minHeap->elements = realloc(minHeap->elements, minHeap->size * sizeof(HeapNode*));
    minHeap_heapify(minHeap, 0);
    return t;
  }

  return NULL;
}

void printMinHeap(MinHeap* minHeap) {
  if(minHeap == NULL) return;
  int i;
  for(i = 0; i < minHeap->size; i++) {
    printf("{'%s', %d}", minHeap->elements[i]->data, minHeap->elements[i]->freq);
  }

  printf("\n");
}

