#include "libs/bintree/bintree.h"
#include "libs/minheap/minheap.h"

#ifndef MAIN_H
#define MAIN_H

typedef struct CodebookNode
{
    char *key;
    char *value;
    CodebookNode *next;
} CodebookNode;

MinHeap *recursiveTokenization(char *path, MinHeap *minHeap);
MinHeap *insertIntoHeap(char *file, MinHeap *minHeap);
void *printCodes(BinTreeNode *root, int arr[], int top, int fd);
int isDelim(char curr);
int isRegFile(const char *path);
void compressFile(char *path, CodebookNode *head);

#endif