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
    if (argc < 3)
    {
        printf("\033[0;31m");
        printf("[ ERROR ] Please make sure to pass in one of following flags: [ -b, -c, -d, and -R for recursive ]\n");
        printf("\033[0m");
        exit(1);
    }

    char flag1 = argv[1][1];
    char flag2 = argv[2][1];

    // checks that first flag is valid
    if (flag1 != 'b' && flag1 != 'c' && flag1 != 'd' && flag1 != 'R')
    {
        printf("\033[0;31m");
        printf("[ ERROR ] Please make sure to pass in one of following flags: [ -b, -c, -d, and -R for recursive ]\n");
        printf("\033[0m");
        exit(1);
    }

    if (argv[2][0] == '-' && (flag2 != 'b' && flag2 != 'c' && flag2 != 'd' && flag2 != 'R'))
    {
        printf("\033[0;31m");
        printf("[ ERROR ] Please make sure to pass in one of following flags: [ -b, -c, -d, and -R for recursive ]\n");
        printf("\033[0m");
        exit(2);
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

    if (minHeap == NULL || minHeap->size == 0)
    {
        printf("\033[0;31m");
        printf("[ ERROR ] Make sure the file you have passed exists and is not empty.\n");
        printf("\033[0m");
        exit(1);
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

        printf("\033[0;32m");
        printf("[ Starting to Build HuffmanCodebook ]\n");
        printf("\033[0m");

        int treeCount = 0;
        int c = 1000;
        while (minHeap->size > 1 && c > 0)
        {
            // printf("minHeap->size = %d\n", minHeap->size);
            char *treeStringHolder = malloc(sizeof(char) * 80);
            c--; //max loops for debugging
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

        printf("\033[0;32m");
        printf("[ Successfully created HuffmanCodebook ]\n");
        printf("\033[0m");
    }
    else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[2], "-d") == 0)
    {
        printf("need to decompress\n");
        //decompress
    }
    else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[2], "-c") == 0)
    {
        printf("\033[0;32m");
        printf("[ Starting Compression ]\n");
        printf("\033[0m");
        //compress

        if (argc >= 3)
        {
            //we need to read in the codebook and create a linked list of all of the key -> value pairs
            char *path = (argc == 4) ? argv[2] : argv[3];
            int codebookFD = open(argc == 4 ? argv[3] : argv[4], O_RDONLY);
            if (codebookFD == -1)
            {
                printf("\033[0;31m");
                printf("[ ERROR ] Codebook does not exist at the path provided. To create a Codebook, use the -b flag \n");
                printf("\033[0m");
                exit(1);
            }

            CodebookNode *codebookHead = malloc(sizeof(CodebookNode));
            char currChar;
            int num_bytes = 0;

            char *currToken = calloc(1000, sizeof(char));
            int currTokenSize = 0;

            int key = 1;
            CodebookNode *ptr = codebookHead;

            int started = 0;

            while (1)
            {
                num_bytes = read(codebookFD, &currChar, 1);

                //this means the file is empty
                if (num_bytes == 0 && started == 0)
                {
                    printf("\033[0;31m");
                    printf("[ ERROR ] Codebook does not exist or is empty. To create a Codebook, use the -b flag \n");
                    printf("\033[0m");
                    exit(1);
                }

                //init conidition override now that we know the file exists and is not empty
                started = 1;

                if (isDelim(currChar) || num_bytes == 0)
                {
                    char *tempCurrToken = calloc((currTokenSize), sizeof(char));
                    strncpy(tempCurrToken, currToken, currTokenSize + 1);

                    if (key == 1)
                    {
                        ptr->key = tempCurrToken;
                        key = 0;
                    }
                    else
                    {
                        ptr->value = tempCurrToken;
                        key = 1;

                        if (num_bytes != 0)
                        {
                            CodebookNode *temp = malloc(sizeof(CodebookNode));
                            ptr->next = temp;
                            ptr = temp;
                        }
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

            // CodebookNode *ptr2 = codebookHead;
            // while (ptr2->next != NULL)
            // {
            //     printf("KEY: %s\n", ptr2->key);
            //     printf("VALUE: %s\n\n", ptr2->value);
            //     ptr2 = ptr2->next;
            // }

            if (strcmp(argv[1], "-R") == 0)
            {
                recursivelyCompress(argv[3], codebookHead);
            }
            else
            {
                //Non recursive compress
                printf("non recursive\n");
                compressFile(path, codebookHead);
            }

            printf("\033[0;32m");
            printf("[ Successfully Finished Compression ]\n");
            printf("\033[0m");
        }
        else
        {
            printf("\033[0;31m");
            printf("[ ERROR ] Improper inputs configuration for Compress. Please enter your command in the format of ./fileCompressor -c <path_to_file_to_compress> <path_to_codebook>");
            printf("\033[0m");
            return 1;
        }
    }

    return 0;
}

void recursivelyCompress(char *path, CodebookNode *head)
{
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
                    compressFile(nextPath, head);
                }

                recursivelyCompress(nextPath, head);
            }
        }

        closedir(directory);
    }
}

void compressFile(char *path, CodebookNode *head)
{

    int len = strlen(path);

    if (path[len - 4] == '.' && path[len - 3] == 'h' && path[len - 2] == 'c' && path[len - 1] == 'z')
    {
        return;
    }

    int fd = open(path, O_RDONLY);

    if (fd == -1)
    {
        printf("\033[0;31m");
        printf("[ ERROR ] Provided path for compression does not exist \n");
        printf("\033[0m");
        exit(1);
    }

    char *newPath = path;
    sprintf(newPath, "%s.hcz", path);
    if (access(newPath, F_OK) != -1)
    {
        remove(newPath);
    }

    int compressedFileFD = open(newPath, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    char currChar;
    int num_bytes = 0;

    int started = 0;

    char *currToken = calloc(1000, sizeof(char));
    int currTokenSize = 0;

    while (1)
    {
        num_bytes = read(fd, &currChar, 1);
        // printf("read: %c\n", currChar);

        if (num_bytes == 0 && currToken == 0 && started == 0)
        {
            printf("\033[0;31m");
            printf("[ ERROR ] Contents of file are empty, nothing to compress \n");
            printf("\033[0m");
            exit(1);
        }

        started = 1;

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

                //if we have gotten here, it it means there is a delim at the start of the file
                // printf("tempString = %s\n", tempString);
                char *replacement = codebook_getKeyByValue(tempString, head);
                if (replacement == NULL)
                {
                    printf("replacement is NULL!\n");
                    exit(1);
                }
                // printf("replacing '%s' with '%s'\n", tempString, replacement);
                write(compressedFileFD, replacement, strlen(replacement));
            }
            else
            {
                char *tempCurrToken = malloc((currTokenSize) * sizeof(char));
                strncpy(tempCurrToken, currToken, currTokenSize + 1);

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
                char *tokenReplacement = codebook_getKeyByValue(tempCurrToken, head);
                char *strReplacement = codebook_getKeyByValue(tempString, head);
                if (tokenReplacement == NULL)
                {
                    printf("tokenReplacement is NULL!\n");
                    exit(1);
                }
                if (strReplacement == NULL)
                {
                    printf("strReplacement is NULL!\n");
                    exit(1);
                }

                // printf("replacing '%s%s' with '%s%s'\n", tempCurrToken, tempString, tokenReplacement, strReplacement);
                write(compressedFileFD, tokenReplacement, strlen(tokenReplacement));
                write(compressedFileFD, strReplacement, strlen(strReplacement));
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

    return;
}

char *codebook_getValueByKey(char *key, CodebookNode *head)
{

    CodebookNode *curr = head;
    while (curr != NULL)
    {
        if (strcmp(key, curr->key) == 0)
        {
            return curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}
char *codebook_getKeyByValue(char *value, CodebookNode *head)
{
    CodebookNode *curr = head;
    while (curr != NULL)
    {
        if (strcmp(value, curr->value) == 0)
        {
            return curr->key;
        }
        curr = curr->next;
    }

    return NULL;
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

    int started = 0;

    char *currToken = calloc(2000, sizeof(char));
    int currTokenSize = 0;

    while (1)
    {
        num_bytes = read(fd, &currChar, 1);
        if (num_bytes == 0 && currToken == 0 && started == 0)
        {
            return minHeap;
        }

        //used in a check to see if the file is empty on the first read, but if we get here the file isn't empty
        //so set the var to 1
        started = 1;
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