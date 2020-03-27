#include<stdio.h>
#include<stdlib.h>

//import custom libs and supporting files
#include "libs/bintree/bintree.h"
#include "libs/minheap/minheap.h"

int main(int argc, char** argv) {

  MinHeap* minHeap = createMinHeap();
  HeapNode* n1 = createHeapNode("a", 100);
  HeapNode* n2 = createHeapNode("b", 50);
  HeapNode* n3 = createHeapNode("c", 75);
  HeapNode* n4 = createHeapNode("d", 24);
  HeapNode* n5 = createHeapNode("e", 17);
  HeapNode* n6 = createHeapNode("f", 10321);

  printMinHeap(minHeap);
  minHeap_insert(minHeap, n1);
  minHeap_insert(minHeap, n2);
  minHeap_insert(minHeap, n3);
  minHeap_insert(minHeap, n4);
  minHeap_insert(minHeap, n5);
  minHeap_insert(minHeap, n6);
  printMinHeap(minHeap);

  HeapNode* t = minHeap_delete(minHeap);  
  printMinHeap(minHeap);

  printf("node deleted = {'%s', %d}\n", t->data, t->freq);

  t = minHeap_search(minHeap, "f");
  printf("node searched = {'%s', %d}\n", t->data, t->freq);

  printf("success\n");
  return 1;
}