/*
Header file for Bintree
*/

//Prevent's double include
#ifndef BINTREE_H
#define BINTREE_H

//HeapNode Struct Definition
typedef struct BinTreeNode
{
  char *data;
  struct BinTreeNode *left;
  struct BinTreeNode *right;
} BinTreeNode;

//Heap Struct Definition
typedef struct BinTree
{
  int size;
  BinTreeNode *head;
} BinTree;

#endif