#include<stdlib.h>
#include<stdio.h>
#include "bintree.h"

/**
 * Create a BinTreeNode from the properties passed in
 * 
 * Removes clutter of mallocing and assigning props in the codebase, instead just call this method with params for readability
 */
BinTreeNode* createBinTreeNode(char* data, BinTreeNode* left, BinTreeNode* right) {
  BinTreeNode* t = malloc(sizeof(BinTreeNode));
  t->data = data;
  t->left = left;
  t->right = right;
  return t;
}

/**
 * Print out BinTree using preorder syntax
 */
void preorderPrintBinTree(BinTreeNode* head) {
  if(head != NULL) {
    printf("%s ", head->data);
    preorderPrintBinTree(head->left);
    preorderPrintBinTree(head->right);
  }
}

/**
 * Print out BinTree using inorder syntax
 */
void inorderPrintBinTree(BinTreeNode* head) {
if(head != NULL) {
    inorderPrintBinTree(head->left);
    printf("%s ", head->data);
    inorderPrintBinTree(head->right);
  }
}

/**
 * Print out BinTree using postorder syntax
 */
void postorderPrintBinTree(BinTreeNode* head) {
  if(head != NULL) {
    postorderPrintBinTree(head->left);
    postorderPrintBinTree(head->right);
    printf("%s ", head->data);
  }
}
