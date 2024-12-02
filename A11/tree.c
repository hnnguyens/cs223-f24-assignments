/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: December 1st, 2024
 * Description: Implements a binary tree.
 ---------------------------------------------*/
#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/**
 * Finds the specified node.
 * @param name 
 * @param root
 */
struct tree_node *find(const char *name, struct tree_node *root) {
  // compare using element type data
  if (root == NULL) {
    return NULL;
  }

  int comparison = strcmp(name, root->data.name);

  if (comparison == 0) {  //same
    return root; // found
  }

  else if (comparison < 0) {
    return find(name, root->left); //recursive
  }

  else {
    return find(name, root->right);
  }
}

//inserts a node into the tree
struct tree_node *insert(const char *name, struct tree_node *root) {
  if (root == NULL) {
    root = (struct tree_node *)malloc(sizeof(struct tree_node));

    if (root == NULL) {
      printf("No space!\n");
      exit(0);
    }

    strncpy(root->data.name, name, sizeof(root->data.name));
    root->data.name[sizeof(root->data.name)] = '\0';
    root->left = NULL;
    root->right = NULL;
  }

  int comparison = strcmp(name, root->data.name);

  //if comparison == 0, then it already exists, no need to insert duplicate

  if (comparison < 0) {
    root->left = insert(name, root->left);
  }

  else if (comparison > 0) {
    root->right = insert(name, root->right);
  }

  return root;
}

//empties out the tree, freeing it's data
void clear(struct tree_node *root) {
  if (root == NULL) { // tree already empty
    return;
  }

  clear(root->left);
  clear(root->right);
  free(root); // free root, clean slate
}

//prints out the contents of the tree
void print(struct tree_node *root) {
  if (root == NULL) { // base case
    return;
  }

  printf("%s\n", root->data.name);
  print(root->left);
  print(root->right);
}

//sorted, in order traversal
void printSorted(struct tree_node *root) {
  if (root == NULL)
  {
    return;
  }

  printSorted(root->left);
  printf("%s\n", root->data.name);
  printSorted(root->right);
}