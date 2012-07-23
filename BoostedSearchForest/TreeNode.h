/* 
 * File:   TreeNode.h
 * Author: alpha
 *
 * Created on July 23, 2012, 9:23 AM
 */

#ifndef TREENODE_H
#define	TREENODE_H

class TreeNode {
public:
    TreeNode();
    virtual ~TreeNode();
    bool isInternal( void );
    bool isLeaf( void );
protected:
    bool internalNode;

};

#endif	/* TREENODE_H */

