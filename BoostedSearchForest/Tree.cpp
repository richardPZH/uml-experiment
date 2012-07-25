/* 
 * File:   Tree.cpp
 * Author: alpha
 * 
 * Created on July 23, 2012, 8:54 AM
 */

#include "Tree.h"

Tree::Tree( const size_t numOfSamples ) {

    root = NULL;
    numFruit = numOfSamples;
    fruit = new int[ numFruit ];

}


Tree::~Tree() {

    //deleteing the root may recursively delete the whole tree

    delete []fruit;
}

