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
  struct BinTreeNode *left;
  struct BinTreeNode *right;
} BinTreeNode;


//Function Prototype for creating new BinTreeNode
BinTreeNode* createBinTreeNode(char* data, BinTreeNode* left, BinTreeNode* right);

//Function Prototype for printing BinTree using preorder syntax
void preorderPrintBinTree(BinTreeNode* head);

//Function Prototype for printing BinTree using inorder syntax
void inorderPrintBinTree(BinTreeNode* head);

//Function Prototype for printing BinTree using postorder syntax
void postorderPrintBinTree(BinTreeNode* head);

#endif