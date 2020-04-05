#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "main.h"
//import custom libs and supporting files
#include "libs/bintree/bintree.h"
#include "libs/minheap/minheap.h"

int main(int argc, char **argv)
{
  char flag1 = argv[1][1];

  // checks that first flag is valid
  if (flag1 != 'b' && flag1 != 'c' && flag1 != 'd' && flag1 != 'R')
  {
    printf("FATAL ERROR: please pass in either -b, -c, -d, or -R as a flag\n");
    exit(0);
  }

  MinHeap *minHeap = createMinHeap();
  if (flag1 != 'R')
  {
    minHeap = insertIntoHeap(argv[2], minHeap);
    // printMinHeap(minHeap);
  }
  else
  {
    minHeap = recursiveTokenization(argv[3], minHeap);
    // printMinHeap(minHeap);
  }

  /**
   * Build up the Huffman Tree
   * 
   * First we need to start pulling off 2 elements at a time from the MinHeap and check if they are HeapNodes that reference BinTreeNodes or they are just holding char and freq
   */

  HeapNode *temp1 = NULL;
  HeapNode *temp2 = NULL;

  // printf("\n\n");
  /**
   * If we are building a codebook
   */
  if (strcmp(argv[1], "-b") == 0 || strcmp(argv[2], "-b") == 0)
  {
    int treeCount = 0;
    int c = 1000;
    while (minHeap->size > 1 && c > 0)
    {
      // printf("minHeap->size = %d\n", minHeap->size);
      char *treeStringHolder = malloc(sizeof(char) * 80);
      c--;        //max loops for debugging
      sleep(0.1); //slow it down so we can see how its building
      sprintf(treeStringHolder, "tree%d", treeCount);

      minHeap_heapsort(minHeap, 0);
      temp1 = minHeap_delete(minHeap);
      minHeap_heapsort(minHeap, 0);
      temp2 = minHeap_delete(minHeap);

      // printf("temp1 = {'%s', %d, isStoringChar = %d}\n", temp1->data, temp1->freq, temp1->isStoringChar);
      // printf("temp2 = {'%s', %d, isStoringChar = %d}\n", temp2->data, temp2->freq, temp2->isStoringChar);
      if (temp1->isStoringChar == 1 && temp2->isStoringChar == 1)
      {
        //They are both holding characters, so we need to build a subtree that contains these elements
        BinTreeNode *left = createBinTreeNode(temp1->data, temp1->freq, NULL, NULL);
        BinTreeNode *right = createBinTreeNode(temp2->data, temp2->freq, NULL, NULL);
        BinTreeNode *parent = createBinTreeNode(treeStringHolder, temp1->freq + temp2->freq, left, right);
        HeapNode *newHeapNode = createHeapNode_TreeNode(parent, parent->data, calcFrequencyOfEntireTree(parent));

        // printf("new heap node contains {'%s', %d} and {'%s', %d} -> {'%s', total freq = %d\n", temp1->data, temp1->freq, temp2->data, temp2->freq, newHeapNode->data, newHeapNode->freq);
        //Add heapnode back to heap
        minHeap_insert(minHeap, newHeapNode);
        treeCount++;
      }
      else if (temp1->isStoringChar == 1)
      {
        //temp1 is char, temp2 is tree
        BinTreeNode *left = createBinTreeNode(temp1->data, temp1->freq, NULL, NULL);
        BinTreeNode *parent = createBinTreeNode(treeStringHolder, temp1->freq + temp2->freq, left, temp2->treeNode);
        HeapNode *newHeapNode = createHeapNode_TreeNode(parent, parent->data, calcFrequencyOfEntireTree(parent));

        // printf("new heap node contains {'%s', %d} and {'%s', %d} -> {'%s', total freq = %d\n", temp1->data, temp1->freq, temp2->data, temp2->freq, newHeapNode->data, newHeapNode->freq);
        //Add heapnode back to heap
        minHeap_insert(minHeap, newHeapNode);
        treeCount++;
      }
      else if (temp2->isStoringChar == 1)
      {
        //temp1 is tree, temp2 is char
        BinTreeNode *right = createBinTreeNode(temp2->data, temp2->freq, NULL, NULL);
        BinTreeNode *parent = createBinTreeNode(treeStringHolder, temp1->freq + temp2->freq, temp1->treeNode, right);
        HeapNode *newHeapNode = createHeapNode_TreeNode(parent, parent->data, calcFrequencyOfEntireTree(parent));

        // printf("new heap node contains {'%s', %d} and {'%s', %d} -> {'%s', total freq = %d\n", temp1->data, temp1->freq, temp2->data, temp2->freq, newHeapNode->data, newHeapNode->freq);
        //Add heapnode back to heap
        minHeap_insert(minHeap, newHeapNode);
        treeCount++;
      }
      else
      {
        //temp1 is tree. temp2 is tree
        BinTreeNode *parent = createBinTreeNode(treeStringHolder, temp1->freq + temp2->freq, temp1->treeNode, temp2->treeNode);
        HeapNode *newHeapNode = createHeapNode_TreeNode(parent, parent->data, calcFrequencyOfEntireTree(parent));
        // printf("new heap node contains {'%s', %d} and {'%s', %d} -> {'%s', total freq = %d\n", temp1->data, temp1->freq, temp2->data, temp2->freq, newHeapNode->data, newHeapNode->freq);
        //Add heapnode back to heap
        minHeap_insert(minHeap, newHeapNode);
        treeCount++;
      }

      minHeap_heapsort(minHeap, 0);
      // printMinHeap(minHeap);

      // printf("-----\n");
    }

    /**
   * The minheap has now been created here, so we need to traverse the tree and write out our Huffman Coding book
   */

    BinTreeNode *root = minHeap->elements[0]->treeNode;
    int *arr = malloc(sizeof(int) * 100);

    if (access("./HuffmanCodebook", F_OK) != -1)
    {
      remove("./HuffmanCodebook");
    }

    int huffmanCodingBookFD = open("HuffmanCodebook", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    printCodes(root, arr, 0, huffmanCodingBookFD);

    printf("Successfully created huffman book\n");
  }
  else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[2], "-d") == 0)
  {
    printf("need to decompress\n");
    //decompress
  }
  else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[2], "-c") == 0)
  {
    printf("need to compress\n");
    //compress

    if (argc >= 4)
    {

      //we need to read in the codebook and create a linked list of all of the key -> value pairs
      int codebookFD = open(argc == 4 ? argv[3] : argv[4], O_RDONLY);
      if (codebookFD == -1)
      {
        exit(1);
      }

      CodebookNode *codebookHead = malloc(sizeof(CodebookNode));
      char currChar;
      int num_bytes = 0;

      char *currToken = malloc(1000 * sizeof(char));
      int currTokenSize = 0;

      int key = 1;
      CodebookNode *ptr = codebookHead;

      while (1) {
        num_bytes = read(codebookFD, &currChar, 1);
        if (isDelim(currChar) || num_bytes == 0) {
          char *tempCurrToken = malloc((currTokenSize) * sizeof(char));
          strncpy(tempCurrToken, currToken, currTokenSize + 1);
          
          if (key == 1) {
            ptr -> key = tempCurrToken;
            key = 0;
          } else {
            ptr -> value = tempCurrToken;
            key = 1;
            
            if (num_bytes != 0) {
              CodebookNode *temp = malloc(sizeof(CodebookNode));
              ptr -> next = temp;
              ptr = temp;
            }
          }

          if (num_bytes == 0) {
            break;
          } else {
            memset(currToken, 0, strlen(currToken));
            currTokenSize = 0;
          }
        } else {
          if (!isDelim(currChar)) {
            currToken[currTokenSize] = currChar;
            currTokenSize++;
          }
        }
      }
      
      CodebookNode *ptr2 = codebookHead;
      while (ptr2 -> next != NULL) {
        printf("KEY: %s\n", ptr2 -> key);
        printf("VALUE: %s\n\n", ptr2 -> value);
        ptr2 = ptr2 -> next;
      }
      
      if (strcmp(argv[2], "-R") == 0)
      {
        char *path = argv[4];
        char nextPath[2000];
        //we need to recursively compress
        DIR *directory;
        struct dirent *entry;

        if ((directory = opendir(path)) != NULL)
        {
          while ((entry = readdir(directory)) != NULL)
          {
            if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
            {
              strcpy(nextPath, path);
              strcat(nextPath, "/");
              strcat(nextPath, entry->d_name);

              if (isRegFile(nextPath))
              {
                compressFile(nextPath, codebookHead);
              }

              recursiveTokenization(nextPath, minHeap);
            }
          }

          closedir(directory);
        }
      }
    }
    else
    {
      printf("[ ERROR ]: Improper inputs configuration for Compress. Please enter your command in the format of ./fileCompressor -c <path_to_file_to_compress> <path_to_codebook>");
      return 1;
    }
  }

  return 0;
}

void compressFile(char *path, CodebookNode *head)
{
  return;
}

void *printCodes(BinTreeNode *root, int arr[], int top, int fd)

{
  // Assign 0 to left edge and recur
  if (root->left)
  {

    arr[top] = 0;
    printCodes(root->left, arr, top + 1, fd);
  }

  // Assign 1 to right edge and recur
  if (root->right)
  {

    arr[top] = 1;
    printCodes(root->right, arr, top + 1, fd);
  }

  if (root->left == NULL && root->right == NULL)
  {
    int i;
    char *temp = malloc(sizeof(char) * 2);
    for (i = 0; i < top; i++)
    {
      // printf("%d", arr[i]);
      sprintf(temp, "%d", arr[i]);
      write(fd, temp, strlen(temp));
    }
    write(fd, "\t", sizeof(char));
    write(fd, root->data, strlen(root->data));
    write(fd, "\n", sizeof(char));
    // printf("\n");
  }
}

MinHeap *recursiveTokenization(char *path, MinHeap *minHeap)
{
  char nextPath[2000];
  DIR *directory;
  struct dirent *entry;

  if ((directory = opendir(path)) != NULL)
  {
    while ((entry = readdir(directory)) != NULL)
    {
      if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
      {
        strcpy(nextPath, path);
        strcat(nextPath, "/");
        strcat(nextPath, entry->d_name);

        if (isRegFile(nextPath))
        {
          minHeap = insertIntoHeap(nextPath, minHeap);
        }

        recursiveTokenization(nextPath, minHeap);
      }
    }

    closedir(directory);
  }

  return minHeap;
}

MinHeap *insertIntoHeap(char *file, MinHeap *minHeap)
{
  int len = strlen(file);

  if (file[len - 4] == '.' && file[len - 3] == 'h' && file[len - 2] == 'c' && file[len - 1] == 'z')
  {
    return minHeap;
  }

  int fd = open(file, O_RDONLY);

  if (fd == -1)
  {
    return minHeap;
  }

  char currChar;
  int num_bytes = 0;

  char *currToken = malloc(2000 * sizeof(char));
  int currTokenSize = 0;

  while (1)
  {
    num_bytes = read(fd, &currChar, 1);
    if (isDelim(currChar) || num_bytes == 0)
    {
      if (isDelim(currChar) && currTokenSize == 0)
      {
        if (num_bytes == 0)
        {
          break;
        }

        // insert the delim into the minheap
        char *tempString = malloc(4 * sizeof(char));
        if (currChar == '\t')
        {
          tempString = "<\\t>";
        }
        else if (currChar == ' ')
        {
          tempString = "<\\s>";
        }
        else if (currChar == '\n')
        {
          tempString = "<\\n>";
        }

        HeapNode *temp2 = minHeap_search(minHeap, tempString);
        if (temp2 == NULL)
        {
          temp2 = createHeapNode(tempString, 1);
          minHeap_insert(minHeap, temp2);
        }
        else
        {
          temp2->freq++;
        }
      }
      else
      {
        char *tempCurrToken = malloc((currTokenSize) * sizeof(char));
        strncpy(tempCurrToken, currToken, currTokenSize + 1);

        // check if token already exists
        HeapNode *temp1 = minHeap_search(minHeap, tempCurrToken);
        if (temp1 == NULL)
        {
          temp1 = createHeapNode(tempCurrToken, 1);
          minHeap_insert(minHeap, temp1);
        }
        else
        {
          temp1->freq++;
        }

        if (num_bytes == 0)
        {
          break;
        }
        else
        {
          memset(currToken, 0, strlen(currToken));
          currTokenSize = 0;
        }

        // insert the delim into the minheap
        char *tempString = malloc(4 * sizeof(char));
        if (currChar == '\t')
        {
          tempString = "<\\t>";
        }
        else if (currChar == ' ')
        {
          tempString = "<\\s>";
        }
        else if (currChar == '\n')
        {
          tempString = "<\\n>";
        }

        HeapNode *temp2 = minHeap_search(minHeap, tempString);
        if (temp2 == NULL)
        {
          temp2 = createHeapNode(tempString, 1);
          minHeap_insert(minHeap, temp2);
        }
        else
        {
          temp2->freq++;
        }
      }
    }
    else
    {
      if (!isDelim(currChar))
      {
        currToken[currTokenSize] = currChar;
        currTokenSize++;
      }
    }
  }

  close(fd);

  return minHeap;
}

// checks if the current character is a delimiter
int isDelim(char curr)
{
  if (curr == ' ' || curr == '\t' || curr == '\n')
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int isRegFile(const char *path)
{
  struct stat statbuf;

  if (stat(path, &statbuf) != 0)
  {
    return 0;
  }

  return S_ISREG(statbuf.st_mode);
}