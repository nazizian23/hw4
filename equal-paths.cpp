#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkPaths(Node* node, int depth, int& leafDepth);

bool equalPaths(Node * root)
{
    // Add your code below
    if(root==nullptr){
        return true; //Empty tree satisfies the condition
    }

    int leafDepth=-1;//Stores the depth of the first leaf that is found
    return checkPaths(root,0,leafDepth);

}



bool checkPaths(Node* node, int depth, int& leafDepth){
    if(node==nullptr){
        return true; //Ignores the null paths in recursion
    }

    //Check if it is a leaf node
    if(node->left==nullptr&&node->right==nullptr){
        if(leafDepth==-1){
            leafDepth=depth;//First leaf that sets the target depth
        }
        return depth==leafDepth;
    }

    bool leftCheck=true;
    bool rightCheck=true;

    //Recurse and check left/right children
    if(node-> left !=nullptr){
        leftCheck=checkPaths(node->left,depth+1,leafDepth);
    }
    if(node-> right !=nullptr){
        rightCheck=checkPaths(node->right,depth+1,leafDepth);
    }

    return leftCheck && rightCheck;




}
