/* 
 * File:   TreeNode.cpp
 * Author: alpha
 * 
 * Created on July 23, 2012, 9:23 AM
 */

#include "TreeNode.h"

TreeNode::TreeNode() {
    myType = LEAF;   //default is a leaf node
    leafL.puivector = NULL;
}

TreeNode::~TreeNode() {
    //if this is an internal node the intL works and the pvector must have a value, delete it //here I find it!
    if( myType == INTERNAL )
    {
        delete intL.pvector;
    }else
    {
        if( NULL != leafL.puivector )
        {
            delete leafL.puivector;
        }
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

bool TreeNode:: setLeaf( void )
{
    myType = LEAF;
}


bool TreeNode:: setInternal( void )
{
    myType = INTERNAL;
}