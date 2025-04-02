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
    void rotateLeft(AVLNode<Key,Value>*x);
    void rotateRight(AVLNode<Key,Value>*x);
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void removeFix(AVLNode<Key,Value>* n, int diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  // TODO
  // If the tree is empty make a new root node
  if(this->root_==nullptr){
    this->root_=new AVLNode<Key,Value>(new_item.first,new_item.second,nullptr);
    return;
  }
  // Traverse the tree to find the insertion point
  AVLNode<Key,Value>*curr=static_cast<AVLNode<Key,Value>*>(this->root_);
  AVLNode<Key,Value>*parent=nullptr;


  while(curr!=nullptr){
    parent=curr;
    if(new_item.first<curr->getKey()){
      curr=curr->getLeft();
    }
    else if(new_item.first>curr->getKey()){
      curr=curr->getRight();
    }
    else{
      //If the key exists overwrite the val and return
      curr->setValue(new_item.second);
      return;
    }
  }
  //Create a node and attatch it to the parent 
  AVLNode<Key,Value>* newNode= new AVLNode<Key,Value>(new_item.first,new_item.second,parent);
  if(new_item.first<parent->getKey()){
    parent->setLeft(newNode);
  }
  else{
    parent->setRight(newNode);
  }
  //Update the parents balance factor and fix the tree
  if(parent->getBalance()!=0){
    parent->setBalance(0);//Balance becomes 0 if it was unbalanced 
  }
  else{
    if(newNode==parent->getLeft()){
      parent->setBalance(-1);//Left subtree grows
    }
    else{
      parent->setBalance(1);//Right subtree grows
    }
    insertFix(parent,newNode);//Calls helper to rebalance
    
  }

}

template<class Key, class Value>
void AVLTree<Key, Value>:: insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>*n)
{
  
  if(p==nullptr){
    return;
  }
  //Get the grandparent node
  AVLNode<Key, Value>* g=p->getParent();
  
  //If no grandparent no need for fixing
  if(g==nullptr){
    return;
  }

  //If parent is left child of grandparent 
  if(p==g->getLeft()){
    g->updateBalance(-1);//left subtree grows and decrease balance
    if(g->getBalance()==0){
      return;//Tree is balanced
    }
    else if(g->getBalance()==-1){
      insertFix(g,p);//Continue fixing 
    }
    else if(g->getBalance()==-2){//Left heavy requires rotation
      if(n==p->getLeft()){
        //single right rotation 
        rotateRight(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      else{
        //left then right rotation
        rotateLeft(p);
        rotateRight(g);
        int b=n->getBalance();
        if(b==-1){
          p->setBalance(0);
          g->setBalance(1);
        }
        else if(b==0){
          p->setBalance(0);
          g->setBalance(0);
        }
        else{
          p->setBalance(-1);
          g->setBalance(0);
        }
        n->setBalance(0);
      }
    }
  }
  //Parent is the right child of grandparent
  else{
    g->updateBalance(1);//Right subtree grows increase the balance
    if(g->getBalance()==0){
      return;//Tree is balanced
    }
    else if(g->getBalance()==1){
      insertFix(g,p);//Continue fixing
    }
    else if(g->getBalance()==2){
      if(n==p->getRight()){
        //Single left rotation
        rotateLeft(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      else{
        //Right then left rotation
        rotateRight(p);
        rotateLeft(g);
        int b=n->getBalance();
        if(b==1){
          p->setBalance(0);
          g->setBalance(-1);
        }
        else if(b==0){
          p->setBalance(0);
          g->setBalance(0);
        }
        else{
          p->setBalance(1);
          g->setBalance(0);
        }
        n->setBalance(0);
      }
    }
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
  //Find the node to remove
  Node<Key,Value>* node=this->internalFind(key);
  if(node==nullptr){
    return;//Key not found nothing to remove
  }
  AVLNode<Key,Value>*n=static_cast<AVLNode<Key,Value>*>(node);
  AVLNode<Key,Value>*p=n->getParent();
  int diff=0;
  //If node has two children swap with predecessor
  if(n->getLeft()!=nullptr && n->getRight()!=nullptr){
    AVLNode<Key,Value>*pred=static_cast<AVLNode<Key,Value>*>(this->predecessor(n));
    nodeSwap(n,pred);
    p=n->getParent();//Update parent after swap
    
  }
  //Determine the child to replace the removed node 
  AVLNode<Key,Value>*child=nullptr;
  if(n->getLeft()!=nullptr){
    child=n->getLeft();
  }
  else if(n->getRight()!=nullptr){
    child=n->getRight();
  }
  
  
  //Update the tree structure
  if(p==nullptr){
    this->root_=child;//Node was root replace with child 
  }
  else if(n==p->getLeft()){
    p->setLeft(child);//Replace left child 
    diff=1;//Left subtree shrank
  }
  else{
    p->setRight(child);//Replace right child
    diff=-1;//Right subtree shrinks
  }
  
  //Update childs parent pointer if child exists
  if(child!=nullptr){
    child->setParent(p);
  }
  delete n;//Free the removed nodes memory 
  removeFix(p,diff);//rebalance the tree starting from parent 

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff)
{
  if(n==nullptr){
    return;
  }
  //Get the parent of the current node
  AVLNode<Key,Value>*p=n->getParent();
  bool isLeftChild=(p!=nullptr&&n==p->getLeft());
  int nextDiff;

  //Left subtree shrank
  if(diff==1){
    n->updateBalance(diff);//Update balance factor 
    int balance=n->getBalance();
    //If balance is 1 tree is balanced
    if(balance==1){
      return;
    }
    //If balance is 0 height decrease
    else if(balance==0){
      if(p!=nullptr){
        if(isLeftChild){
          nextDiff=1;
        }
        else{
          nextDiff=-1;
        }
        removeFix(p,nextDiff);
      }
      
      return;
    }
    //If balance is 2 tree is right heavy
    else if(balance==2){
      AVLNode<Key,Value>* r =n->getRight();
      int rb=r->getBalance();
      //Single left rotation
      if(rb>=0){
        rotateLeft(n);
        if(rb==0){
          //Height unchanged, adjust balances and recurse
          n->setBalance(1);
          r->setBalance(-1);
          return;
        }
        else{
          //Height decrease, adjust balances and recurse
          n->setBalance(0);
          r->setBalance(0);
        
          if(p!=nullptr){
            if(isLeftChild){
              nextDiff=1;
            }
            else{
              nextDiff=-1;
            }
            removeFix(p,nextDiff);
          }
        }
      }
      //Double rotation
      else{
        AVLNode<Key,Value>* rl =r->getLeft();
        int rlb=rl->getBalance();
        rotateRight(r);
        rotateLeft(n);
        //Adjust balances based on rls balance
        if(rlb==0){
          n->setBalance(0);
          r->setBalance(0);
        }
        else if(rlb==1){
          n->setBalance(-1);
          r->setBalance(0);
        }
        else{
          n->setBalance(0);
          r->setBalance(1);

        }
        rl->setBalance(0);
        //Recurse to parent as height may have changed
        if(p!=nullptr){
          if(isLeftChild){
            nextDiff=1;
          }
          else{
            nextDiff=-1;
          }
          removeFix(p,nextDiff);
        }
      }
    }
  }
  //Right subtree shrank
  else{
    //Update the balance factor
    n->updateBalance(diff);
    int balance=n->getBalance();
    //If balance is -1 tree is balanced and height stays the same
    if(balance==-1){
      return;
    }
    //If balance is 0 height decrease
    else if(balance==0){
      if(p!=nullptr){
        if(isLeftChild){
          nextDiff=1;
        }
        else{
          nextDiff=-1;
        }
        removeFix(p,nextDiff);
      }
      
      return;
    }
    //If balance is -2 tree is left heavy needs rotation
    else if(balance==-2){
      AVLNode<Key,Value>* l =n->getLeft();
      int lb=l->getBalance();
      //Single right rotation 
      if(lb<=0){
        rotateRight(n);
        if(lb==0){
          //Height unchanged adjust balances
          n->setBalance(-1);
          l->setBalance(1);
          return;
        }
        else{
          //Height decreased adjust balances and recurse
          n->setBalance(0);
          l->setBalance(0);
        
          if(p!=nullptr){
            if(isLeftChild){
              nextDiff=1;
            }
            else{
              nextDiff=-1;
            }
            removeFix(p,nextDiff);
          }
        
        }
          
      }
      //Double rotation left-right
      else{
        AVLNode<Key,Value>* lr =l->getRight();
        int lrb=lr->getBalance();
        rotateLeft(l);
        rotateRight(n);
        //Adjust balances based on lrs balance
        if(lrb==0){
          n->setBalance(0);
          l->setBalance(0);
        }
        else if(lrb==-1){
          n->setBalance(1);
          l->setBalance(0);
        }
        else{
          n->setBalance(0);
          l->setBalance(-1);

        }
        lr->setBalance(0);
        //Recurse to parent as height may have changed
        if(p!=nullptr){
          if(isLeftChild){
            nextDiff=1;
          }
          else{
            nextDiff=-1;
          }
          removeFix(p,nextDiff);
        }
      }
    }
  }
  
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft( AVLNode<Key,Value>* x)
{
  AVLNode<Key,Value>*y=x->getRight();
  x->setRight(y->getLeft());
  
  if(y->getLeft()!=nullptr){
    y->getLeft()->setParent(x);//Update the parent pointer of y's left child
  }
  
  y->setParent(x->getParent());//Set y's parent to x's parent 
  if(x->getParent()==nullptr){
    this->root_=y;//If x was root y becomes new root
  }
  else if(x==x->getParent()->getLeft()){
    x->getParent()->setLeft(y);//Update parents left child to y

  }
  else{
    x->getParent()->setRight(y);//Update parents right child to y
  }
  y->setLeft(x);//X becomes ys left child
  x->setParent(y);//Update xs parent to y

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight( AVLNode<Key,Value>* x)
{
  AVLNode<Key,Value>*y=x->getLeft();
  x->setLeft(y->getRight());//Move ys right child to xs left
  if(y->getRight()!=nullptr){
    y->getRight()->setParent(x);//Update parent pointer of ys right child
  }
  y->setParent(x->getParent());//Set ys parent to xs parent
  if(x->getParent()==nullptr){
    this->root_=y;//If x was root y becomes new root
  }
  else if(x==x->getParent()->getRight()){
    x->getParent()->setRight(y);//Update parents right child to y

  }
  else{
    x->getParent()->setLeft(y);//Update parents left child to y
  }
  y->setRight(x);// x becomes ys right child
  x->setParent(y);//Update xs parent to y

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
