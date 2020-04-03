/*
Header file for Bintree
*/

//Prevent's double include
#ifndef BINTREE_H
#define BINTREE_H

//BinTreeNode Struct Definition
typedef struct BinTreeNode
{
  char *data;
  int freq;
  struct BinTreeNode *left;
  struct BinTreeNode *right;
  int encoded;
  char *encoding;
} BinTreeNode;

//Function Prototype for creating new BinTreeNode
BinTreeNode *createBinTreeNode(char *data, int freq, BinTreeNode *left, BinTreeNode *right);

//Function Prototype for getting frequency of all children in a tree
int calcFrequencyOfChildren(BinTreeNode *root);

//Function Prototype for getting frequency of all chars in a tree
int calcFrequencyOfEntireTree(BinTreeNode *root);

//Function Prototype for printing BinTree using preorder syntax
void preorderPrintBinTree(BinTreeNode *head);

//Function Prototype for printing BinTree using inorder syntax
void inorderPrintBinTree(BinTreeNode *head);

//Function Prototype for printing BinTree using postorder syntax
void postorderPrintBinTree(BinTreeNode *head);

#endif