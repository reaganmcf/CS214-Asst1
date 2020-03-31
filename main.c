#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

//import custom libs and supporting files
#include "libs/bintree/bintree.h"
#include "libs/minheap/minheap.h"

MinHeap* recursiveTokenization(char* path, MinHeap* minHeap);
MinHeap* insertIntoHeap(char* file, MinHeap* minHeap);
int isDelim(char curr);
int isRegFile(const char* path);

int main(int argc, char** argv) {
  char flag1 = argv[1][1];

  // checks that first flag is valid
  if (flag1 != 'b' && flag1 != 'c' && flag1 != 'd' && flag1 != 'R') {
    printf("FATAL ERROR: please pass in either -b, -c, -d, or -R as a flag\n");
    exit(0);
  }
  
  MinHeap* minHeap = createMinHeap();
  if (flag1 != 'R') {
    minHeap = insertIntoHeap(argv[2], minHeap);
    printMinHeap(minHeap);
  } else {
    minHeap = recursiveTokenization(argv[3], minHeap);
    printMinHeap(minHeap);
  }

  return 1;

  /*
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
  */
}

MinHeap* recursiveTokenization(char* path, MinHeap* minHeap) {
  char nextPath[2000];
  DIR* directory;
  struct dirent* entry;

  if ((directory = opendir(path)) != NULL) {
    while ((entry = readdir(directory)) != NULL) {
      if ((strcmp(entry -> d_name, ".") != 0) && (strcmp(entry -> d_name, "..") != 0)) {
        strcpy(nextPath, path);
        strcat(nextPath, "/");
        strcat(nextPath, entry -> d_name);

        if (isRegFile(nextPath)) {
          minHeap = insertIntoHeap(nextPath, minHeap);
        }

        recursiveTokenization(nextPath, minHeap);
      }
    }

    closedir(directory);
  }

  return minHeap;
}

MinHeap* insertIntoHeap(char* file, MinHeap* minHeap) {
  int fd = open(file, O_RDONLY);
  
  if (fd == -1) {
    return minHeap;
  }

  char currChar;
  int num_bytes = 0;

  char *currToken = malloc(2000 * sizeof(char));
  int currTokenSize = 0;

  while (1) {
    num_bytes = read(fd, &currChar, 1);
    if (isDelim(currChar) || num_bytes == 0) {
      if (isDelim(currChar) && currTokenSize == 0) {
        if (num_bytes == 0) {
          break;
        }

        // insert the delim into the minheap
        char* tempString = malloc(4 * sizeof(char));
        if (currChar == '\t') {
          tempString = "<\\t>";
        } else if (currChar == ' ') {
          tempString = "<\\s>";
        } else if (currChar == '\n') {
          tempString = "<\\n>";
        }

        HeapNode* temp2 = minHeap_search(minHeap, tempString);
        if (temp2 == NULL) {
          temp2 = createHeapNode(tempString, 1);
          minHeap_insert(minHeap, temp2);
        } else {
          temp2 -> freq++;
        }
      } else {
        char *tempCurrToken = malloc((currTokenSize) * sizeof(char));
        strncpy(tempCurrToken, currToken, currTokenSize + 1);

        // check if token already exists
        HeapNode* temp1 = minHeap_search(minHeap, tempCurrToken);
        if (temp1 == NULL) {
          temp1 = createHeapNode(tempCurrToken, 1);
          minHeap_insert(minHeap, temp1);
        } else {
          temp1 -> freq++;
        }

        if (num_bytes == 0) {
          break;
        } else {
          memset(currToken, 0, strlen(currToken));
          currTokenSize = 0;
        }

        // insert the delim into the minheap
        char* tempString = malloc(4 * sizeof(char));
        if (currChar == '\t') {
          tempString = "<\\t>";
        } else if (currChar == ' ') {
          tempString = "<\\s>";
        } else if (currChar == '\n') {
          tempString = "<\\n>";
        }

        HeapNode* temp2 = minHeap_search(minHeap, tempString);
        if (temp2 == NULL) {
          temp2 = createHeapNode(tempString, 1);
          minHeap_insert(minHeap, temp2);
        } else {
          temp2 -> freq++;
        }
      } 
    } else {
      if (!isDelim(currChar)) {
        currToken[currTokenSize] = currChar;
        currTokenSize++;
      }
    }
  }

  close(fd);

  return minHeap;
}

// checks if the current character is a delimiter
int isDelim(char curr) {
  if (curr == ' ' || curr == '\t' || curr == '\n') {
    return 1;
  } else {
    return 0;
  }
}

int isRegFile(const char* path) {
  struct stat statbuf;

  if (stat(path, &statbuf) != 0) {
    return 0;
  }

  return S_ISREG(statbuf.st_mode);
}