#include "libs/bintree/bintree.h"
#include "libs/minheap/minheap.h"

#ifndef MAIN_H
#define MAIN_H

typedef struct CodebookNode
{
    char *key;
    char *value;
    struct CodebookNode *next;
} CodebookNode;

MinHeap *recursiveTokenization(char *path, MinHeap *minHeap);
MinHeap *insertIntoHeap(char *file, MinHeap *minHeap);
void *writeCodes(BinTreeNode *root, int arr[], int top, int fd);
void freeBinTree(BinTreeNode *node);
int isDelim(char curr);
int isRegFile(const char *path);
void recursivelyCompress(char *path, CodebookNode *head);
void compressFile(char *path, CodebookNode *head);
void recursivelyDecompress(char *path, CodebookNode *head);
void decompressFile(char *path, CodebookNode *head);
char *codebook_getValueByKey(char *key, CodebookNode *head);
char *codebook_getKeyByValue(char *value, CodebookNode *head);

#endif