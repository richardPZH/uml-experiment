/* 
 * File:   TreeNode.cpp
 * Author: alpha
 * 
 * Created on July 23, 2012, 9:23 AM
 */

#include "TreeNode.h"

TreeNode::TreeNode() {
}

TreeNode::~TreeNode() {
}

bool TreeNode:: isInternal( void )
{
    return internalNode;
}

bool TreeNode:: isLeaf( void )
{
    return !( internalNode );
}