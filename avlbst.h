#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    int getHeight(AVLNode<Key, Value>* node) const;
    void rightRotate(AVLNode<Key, Value>* node);
    void leftRotate(AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    //check if key alr exist
    AVLNode<Key, Value>* exist = static_cast<AVLNode<Key, Value>*>(this->internalFind(new_item.first));
    if(exist!=nullptr){
      //exist so update val
      exist->setValue(new_item.second);
      return;
    }

    //making new node
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    
    //empty tree
    if(this->root_ == nullptr){
      this->root_ = newNode;
      return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;
    while(curr!=nullptr){
      parent = curr;
      if(new_item.first < curr->getKey()){
        curr = curr->getLeft();
      }
      else{
        curr = curr->getRight();
      }
    }
      newNode->setParent(parent);
      if(new_item.first < parent->getKey()){
        parent->setLeft(newNode);
      }
      else{
        parent->setRight(newNode);
      }
      
      //rebalancing walking upward from parent
      curr = parent;
      while(curr!=nullptr){
        int leftHeight = getHeight(curr->getLeft());
        int rightHeight = getHeight(curr->getRight());
        curr->setBalance(static_cast<int8_t>(rightHeight - leftHeight));

        //check if unbalanced
      if(curr->getBalance() == -2){
        //left heavy
        AVLNode<Key, Value>* leftChild = curr->getLeft();
        if(leftChild != nullptr && leftChild->getBalance() <= 0){
          //left-left
          rightRotate(curr);
        }
        else{
          //left-right
          if(leftChild != nullptr){
            leftRotate(leftChild);
          }
          rightRotate(curr);
        }
      }
      else if(curr->getBalance() == 2){
        //right heavy
        AVLNode<Key, Value>* rightChild = curr->getRight();
        if(rightChild != nullptr && rightChild->getBalance() >= 0){
          //right-right
          leftRotate(curr);
        }
        else{
          //right-left
          if(rightChild != nullptr){
            rightRotate(rightChild);
          }
          leftRotate(curr);
        }
      }
    curr = curr->getParent();
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    //find node we're going to remove
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(nodeToRemove == nullptr){
      return;
    }

    //remember pos we start balancing from
    AVLNode<Key, Value>* nodeStart = nullptr;

    // 2 children: swap w pred
    if(nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() != nullptr){
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove));
      nodeSwap(nodeToRemove, pred);
    }
    
    //node w/ only 0 or 1 child
    AVLNode<Key, Value>* parent = nodeToRemove->getParent();
    nodeStart = parent;

    AVLNode<Key, Value>* child = nullptr;
    if(nodeToRemove->getLeft() != nullptr){
      child = nodeToRemove->getLeft();
    }
    else{
      child = nodeToRemove->getRight();
    }

    //connecting child t oaprent
    if(child!=nullptr){
      child->setParent(parent);
    }

    //updating parents child ptr
    if(parent == nullptr){
      //delte root
      this->root_ = child;
    }
    else if(parent->getLeft() == nodeToRemove){
      parent->setLeft(child);
    }
    else{
      parent->setRight(child);
    }

    delete nodeToRemove;

    //rebalance tree from bottom up
    AVLNode<Key, Value>* curr = nodeStart;
    while(curr != nullptr){
      //udpate balance
      int leftHeight = getHeight(curr->getLeft());
      int rightHeight = getHeight(curr->getRight());
      curr->setBalance(static_cast<int8_t>(rightHeight - leftHeight));

      //check if unabalcned
      if(curr->getBalance() == -2){
        AVLNode<Key, Value>* leftChild = curr->getLeft();
        if(leftChild != nullptr && leftChild->getBalance() <= 0){
          rightRotate(curr);
        }
        else{
          if(leftChild != nullptr){
            leftRotate(leftChild);
          }
          rightRotate(curr);
        }
      }
      else if(curr -> getBalance() == 2){
        AVLNode<Key, Value>* rightChild = curr->getRight();
        if(rightChild != nullptr && rightChild->getBalance() >= 0){
          leftRotate(curr);
        }
        else{
          if(rightChild != nullptr){
            rightRotate(rightChild);
          }
          leftRotate(curr);
        }
      }
      curr = curr->getParent();
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

//helper func to get height
template<class Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key, Value>* node) const
{
  if(node == nullptr){
    return -1; //empty tree = height -1
  }
  int leftHeight = getHeight(node->getLeft());
  int rightHeight = getHeight(node->getRight());
  return 1 + std::max(leftHeight, rightHeight);
}

//left-left right rotation
template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* node) 
{
  if(node == nullptr){
    return;
  }

  AVLNode<Key, Value>* leftChild = node->getLeft();
  if(leftChild == nullptr){ //cant rotate
    return; 
  }
  AVLNode<Key, Value>* parent = node->getParent();

  //moving left childs right subtree to nodes left side
  AVLNode<Key, Value>* leftR = leftChild->getRight();
  node->setLeft(leftR);
  if(leftR!=nullptr){
    leftR->setParent(node);
  }

  //noden ow right child of leftChild
  leftChild->setRight(node);
  node->setParent(leftChild);

  //connecting leftChild w/ parent
  leftChild->setParent(parent);
  if(parent==nullptr){
    this->root_ = leftChild;
  }
  else if(parent->getLeft() == node){
    parent->setLeft(leftChild);
  }
  else{
    parent->setRight(leftChild);
  }
  
  //rebalancing node n leftChild using hieight
  int nodeLH = getHeight(node->getLeft());
  int nodeRH = getHeight(node->getRight());
  node->setBalance(static_cast<int8_t>(nodeRH - nodeLH));

  int leftLH = getHeight(leftChild->getLeft());
  int leftRH = getHeight(leftChild->getRight());
  leftChild->setBalance(static_cast<int8_t>(leftRH - leftLH));
}

//right-right left rotation
template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* node) 
{
  if(node==nullptr){
    return;
  }
  AVLNode<Key, Value>* rightChild = node->getRight();
  if(rightChild == nullptr){//cant rotate
    return; 
  }
  AVLNode<Key, Value>* parent = node->getParent();

  //moving rightchilds left subtree to the right
  AVLNode<Key, Value>* rightL = rightChild->getLeft();
  node->setRight(rightL);
  if(rightL!= nullptr){
    rightL->setParent(node);
  }

  //setting node as the left child of rightChild
  rightChild->setLeft(node);
  node->setParent(rightChild);

  //connecting righChild w parent
  rightChild->setParent(parent);
  if(parent==nullptr){
    this->root_ = rightChild;
  }
  else if(node == parent->getLeft()){
    parent->setLeft(rightChild);
  }
  else{
    parent->setRight(rightChild);
  }
  //recalculating balacnes for nodes n rightchilds
  int nodeLH = getHeight(node->getLeft());
  int nodeRH = getHeight(node->getRight());
  node->setBalance(static_cast<int8_t>(nodeRH - nodeLH));

  int rightLH = getHeight(rightChild->getLeft());
  int rightRH = getHeight(rightChild->getRight());
  rightChild->setBalance(static_cast<int8_t>(rightRH - rightLH));
}
#endif
