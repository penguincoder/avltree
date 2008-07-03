/* A balanced binary search tree(AVLTree) implementation
 * Written by Coleman
 * Released under GNU GPL Version 2 or later(http://www.gnu.org/)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "Element.h"
#include "Tree.h"

#define MAXELEMENTSIZE 500000

int calcsize(TreeNode *tnode)    {
    long result = 0;
    if(tnode == NULL)
        return 0;
    result = sizeof(tnode);
    result +=(long)sizeof(Element);
    result +=(long)sizeof(char) * strlen(tnode->Name);
    result += calcsize(tnode->Left);
    result += calcsize(tnode->Right);
    return result;
}

void printTree(TreeNode *start, long level)
{
    long i = 0;
    if(start == NULL)
        return;
    printf("%s -> %ld, Balance %d\n", start->Name,
				*start->Item, start->Balance);
    for(i = 0; i < level; i++)
        printf(" ");
    printf("Left: ");
    printTree(start->Left, level + 1);
    printf("\n");
    for(i = 0; i < level; i++)
        printf(" ");
    printf("Right: ");
    printTree(start->Right, level + 1);
    if(level == 0)
        printf("\n");
}

long breadth(TreeNode *start)
{
    if(start == NULL)
        return (long)0;
    if(start->Left == NULL && start->Right == NULL)
        return (long)1;
    return (long)(breadth(start->Left) + breadth(start->Right));
}

int main(int argc, char** argv)
{
    TreeNode *tree = NULL, *newTree = NULL;
    Element newItem = NULL;
    char *itemName = NULL;
    long i = 0, topsize = 0, tick = 0, last = 0;
    long height = 0, oldheight = 0;
    
    /* get the command argument, used for a custom size parameter */
    if(argc == 2)
        topsize = atol(argv[1]);
    if(topsize == 0 || topsize > MAXELEMENTSIZE)
        topsize = 25;
    tick = topsize / 20;
    
    srand48((long)time(NULL));
    printf("Start  -->  Finished\n");
    for(i = 0; i < topsize; i++) {
        newItem = malloc(sizeof(Element));
        itemName = malloc(sizeof(char) * 11);
        sprintf(itemName, "Name%ld", i);
        do  {
            *newItem = lrand48();
            newTree = Insert(tree, newItem, itemName);
            if(newTree == NULL) {
                if(last == *newItem)    {
                    free(itemName);
                    free(newItem);
                    printf("going to try and skip: %ld\n", *newItem);
                    break;
                } else
                    last = *newItem;
            }
        } while(newTree == NULL);
        if(i % tick == 0)   {
            printf(".");
            fflush(stdout);
        }
        tree = newTree;
    }
    printf("\n");
    
    /* test out the delete functionality */
    printf("\nGoing to delete the rootNode: %ld\n", *tree->Item);
    newTree = DeleteByElement(tree, tree->Item);
    if(newTree == NULL)
        printf("failure!\n");
    else {
        tree = newTree;
        printf("success! new root is %ld\n", *tree->Item);
    }
    printf("\nGoing to delete leftMost child... ");
    for(newTree = tree;
        newTree != NULL && newTree->Left != NULL;
        newTree = newTree->Left)    {}
    if(newTree != NULL) {
        printf("found item: %ld\n", *newTree->Item);
        newTree = DeleteByElement(tree, newTree->Item);
        if(newTree == NULL)
            printf("failure!\n");
        else {
            printf("success! new root: %ld\n", *newTree->Item);
            tree = newTree;
        }
    }
    /* duplicated from left side */
    printf("\nGoing to delete rightMost child... ");
    for(newTree = tree;
        newTree != NULL && newTree->Right != NULL;
        newTree = newTree->Right)   {}
    if(newTree != NULL) {
        printf("found item: %ld\n", *newTree->Item);
        newTree = DeleteByElement(tree, newTree->Item);
        if(newTree == NULL)
            printf("failure!\n");
        else {
            printf("success! new root: %ld\n", *newTree->Item);
            tree = newTree;
        }
    }
    printf("\n");
	
    if(topsize <= 25)
        printTree(tree, 0);
    printf("sizeof(TreeNode) %d\n",(int)sizeof(TreeNode));
    printf("sizeof(Element) %d\n",(int)sizeof(Element));
    printf("sizeof(tree) %d\n", calcsize(tree));
    printf("breadth of left tree: %ld right subtree: %ld\n",
           breadth(tree->Left), breadth(tree->Right));
    oldheight = Height(tree->Left);
    printf("height of left subtree: %ld right subtree: %ld\n",
           oldheight, Height(tree->Right));

    last = (long)(LONG_MAX / 2);
    for(i = 0; i < 3; i++)  {
        printf("\n");
        height = lrand48() % oldheight;
        printf("Going to delete a node at depth (max depth %ld): %ld\n",
                oldheight, height);
        newTree = tree;
        do  {
            if(newTree->Left == NULL)   {
                if(newTree->Right == NULL)
                    break;
                tick = last + 1;
            } else if(newTree->Right == NULL)
                tick = last - 1;
            else
                tick = lrand48();
            newTree = (tick >= last ? newTree->Right : newTree->Left);
            height--;
        } while(height > 0);
        printf("Found a node to delete with Name '%s' and Item %ld\n",
                newTree->Name, *newTree->Item);
        newTree = DeleteByElement(tree, newTree->Item);
        if(newTree == NULL)
            printf("failure!\n");
        else {
            printf("success! new root: %ld\n", *newTree->Item);
            tree = newTree;
        }
        oldheight = (int)(oldheight * 0.6);
    }
    
    MakeEmpty(tree);
    return 0;
}
