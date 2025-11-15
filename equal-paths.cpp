#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif
#include <iostream>
#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
//helper function to check if the curr node is a leaf
bool isLeaf(Node* node){
  //curr node pointing to nothing
  if(node == nullptr){
    return false;
  }
  //other scenarior: making sure the curr node has no children
  return (node->left == nullptr && node->right == nullptr);
}

//hlper fun to get height
int getHeight(Node* node){
  if(node == nullptr){
    return 0;
  }

  int leftHeight = getHeight(node->left);
  int rightHeight = getHeight(node->right);

  if(leftHeight > rightHeight){
    return leftHeight + 1;
  }
  else{
    return rightHeight +1;
  }
}

//checking whether or not the paths are equal
bool checkEqual(Node* node, int currDepth, int& leafDepth){
  if(node== nullptr){
    return true;
  }

  if(isLeaf(node)){
    //no leaf found yet
    if(leafDepth == -1){
      //set reference depth after findign first leaf
      leafDepth = currDepth;
      return true;
    }
    else{
      //chec kif curr depth = the ref depth
      return (currDepth == leafDepth);
    }
  }

  //recursively check left n right subtrees
  bool leftEqual = checkEqual(node->left, currDepth + 1, leafDepth);
  if(!leftEqual){ //stopping early
    return false;
  }
  bool rightEqual = checkEqual(node->right, currDepth + 1, leafDepth);

  return leftEqual && rightEqual;
}

bool equalPaths(Node * root)
{
    // Add your code below
  if(root == nullptr){
    return true;
  }

  int leafDepth = -1; //havent found leaf yet
  return checkEqual(root, 1, leafDepth);
}

