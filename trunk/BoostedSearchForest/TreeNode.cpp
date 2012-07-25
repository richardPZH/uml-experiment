/* 
 * File:   TreeNode.cpp
 * Author: alpha
 * 
 * Created on July 23, 2012, 9:23 AM
 */

#include "TreeNode.h"

TreeNode::TreeNode() {
    myType = LEAF;   //default is a leaf node
}

TreeNode::~TreeNode() {
    //if this is an internal node the intL works and the pvector must have a value, delete it
    if( myType == INTERNAL )
    {
        delete intL.pvector;
    }
}

bool TreeNode:: isInternal( void )
{
    return ( myType == INTERNAL );
}

bool TreeNode:: isLeaf( void )
{
    return ( myType == LEAF );
}