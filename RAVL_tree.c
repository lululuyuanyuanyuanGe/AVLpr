/*
 *  RAVL (augmented with Rank AVL) tree implementation.
 *  Author (starter code): Anya Tafliovich.
 *  Based on materials developed by F. Estrada.
 */

#include "RAVL_tree.h"

/*************************************************************************
 ** Suggested helper functions
 *************************************************************************/

/* Returns the height (number of nodes on the longest root-to-leaf path) of
 * the tree rooted at node 'node'. Returns 0 if 'node' is NULL.  Note: this
 * should be an O(1) operation.
 */
int height(RAVL_Node* node){
  if (node == NULL) {
    return 0;
  }
  return node->height;
};

/* Returns the size (number of nodes) of the tree rooted at node 'node'.
 * Returns 0 if 'node' is NULL.  Note: this should be an O(1) operation.
 */
int size(RAVL_Node* node){
  if (node == NULL) {
    return 0;
  }
  return node->size;
};

/* Updates the height of the tree rooted at node 'node' based on the heights
 * of its children. Note: this should be an O(1) operation.
 */
void updateHeight(RAVL_Node* node){
  if (node == NULL) {
    return;
  }

  int leftHeight = height(node->left);
  int rightHeight = height(node->right);

  node->height = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
};

/* Updates the size of the tree rooted at node 'node' based on the sizes
 * of its children. Note: this should be an O(1) operation.
 */
void updateSize(RAVL_Node* node){
  if (node == NULL) {
    return;
  }

  int leftSize = (node->left) ? node->left->size : 0;
  int rightSize = (node->right) ? node->right->size : 0;

  node->size = 1 + leftSize + rightSize;
};

/* Returns the balance factor (height of left subtree - height of right
 * subtree) of node 'node'. Returns 0 if node is NULL.  Note: this should be
 * an O(1) operation.
 */
int balanceFactor(RAVL_Node* node){
  if (node == NULL) {
    return;
  }

  int leftHeight = height(node->left);
  int rightHeight = height(node->right);

  return leftHeight - rightHeight;
};

/* Returns the result of performing the corresponding rotation in the RAVL
 * tree rooted at 'node'.
 */
// single rotations: right/clockwise
RAVL_Node* rightRotation(RAVL_Node* node){
  if (node == NULL || node->left == NULL) {
    return node;
  }

  RAVL_Node* T = node->left;
  node->left = T->right;
  T->right = node;

  // Update height and size in the tree:
  updateHeight(node);
  updateSize(node);
  updateHeight(T);
  updateSize(T);
  return T;
};

// single rotations: left/counter-clockwise
RAVL_Node* leftRotation(RAVL_Node* node){
  if (node == NULL || node->right == NULL) {
    return node;
  }

  RAVL_Node* T = node->right;
  node->right = T->left;
  T->left = node;

  // Update height and size in the tree:
  updateHeight(node);
  updateSize(node);
  updateHeight(T);
  updateSize(T);
  return T;
};

// double rotation: right/clockwise then left/counter-clockwise
RAVL_Node* rightLeftRotation(RAVL_Node* node){
  if (node == NULL || node->right == NULL) {
    return node;
  }
  node->right = rightRotation(node->right);
  return leftRotation(node);
};

// double rotation: left/counter-clockwise then right/clockwise
RAVL_Node* leftRightRotation(RAVL_Node* node){
  if (node == NULL || node->left == NULL) {
    return node;
  }
  node->left = leftRotation(node->left);
  return rightRotation(node);
};

/* Returns the parent node of successor node of 'node'. */
RAVL_Node* successor(RAVL_Node* node){
  RAVL_Node* rTree = node->right;
  RAVL_Node* pNode = node->right;
  if (rTree) {
    while (rTree->left != NULL){
      pNode = rTree;
      rTree = rTree->left;
    }
    return pNode;
  }
  return pNode;
};

/* Creates and returns an RAVL tree node with key 'key', value 'value', height
 * and size of 1, and left and right subtrees NULL.
 */
RAVL_Node* createNode(int key, void* value){
  RAVL_Node* node = (RAVL_Node*)malloc(sizeof(RAVL_Node));
  if(node == NULL) {
    return NULL;
  }
  node->key = key;
  node->value = value;
  node->height = 1;
  node->size = 1;
  node->right = NULL;
  node->left = NULL;

  return node;
};

/* Get balance factor */
int balanceFactor(RAVL_Node* node) {
  return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

/* Balance the tree */
RAVL_Node* balanceTree(RAVL_Node* node) {
  updateHeight(node);
  updateSize(node);

  if(node == NULL) {
    return node;
  }
  

  int balance = balanceFactor(node);
  int balanceL = balanceFactor(node->left);
  int balanceR = balanceFactor(node->right);
  
  
  if (balance > 1 && balanceL > 0) {
    node = rightRotation(node);
  }

  else if(balance > 1 && balanceL < 0) {
    node = leftRightRotation(node);
  }

  else if(balance < -1 && balanceR < 0) {
    node = leftRotation(node);
  }
  else if(balance < -1 && balanceR > 0){
    node = rightLeftRotation(node);
  }
  return node;
}

/*************************************************************************
 ** Provided functions
 *************************************************************************/

void printTreeInorder_(RAVL_Node* node, int offset) {
  if (node == NULL) return;
  printTreeInorder_(node->right, offset + 1);
  printf("%*s %d [%d / %d]\n", offset, "", node->key, node->height, node->size);
  printTreeInorder_(node->left, offset + 1);
}

void printTreeInorder(RAVL_Node* node) { printTreeInorder_(node, 0); }

void deleteTree(RAVL_Node* node) {
  if (node == NULL) return;
  deleteTree(node->left);
  deleteTree(node->right);
  free(node);
}

/*************************************************************************
 ** Required functions
 ** Must run in O(log n) where n is the number of nodes in a tree rooted
 **  at 'node'.
 *************************************************************************/

RAVL_Node* search(RAVL_Node* node, int key) { 
  if(node == NULL) {
    return NULL;
  }
  if (node->key == key) {
    return node;
  }

  else if (node->key > key)
  {
    return search(node->left, key);
  }
  
  return search(node->right, key);}

RAVL_Node* insert(RAVL_Node* node, int key, void* value) {
  // Case 1 tree is empty: 
  if (node == NULL) {
    return createNode(key, value);
  }

  // Case 2 tree is not empty:
  if (node->key < key) {
    node->right = insert(node->right, key, value);
  }
  
  else if (node->key > key) {
    node->left = insert(node->left, key, value);
  }

  else { // No duplicate key allowed
    return node;
  }
}

RAVL_Node* delete(RAVL_Node* node, int key) {
  RAVL_Node* nodeD = search(node, key);
  if (nodeD) {
    RAVL_Node* successor_pNode = successor(nodeD); 
  }

 }

int rank(RAVL_Node* node, int key) { return NOTIN; }

RAVL_Node* findRank(RAVL_Node* node, int rank) { return NULL; }
