/* 
 * File:   BSF.h
 * Author: alpha
 *
 * Created on July 23, 2012, 8:49 AM
 */

#ifndef BSF_H
#define	BSF_H

#include "Tree.h"
#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

class BSF {
public:
    BSF( const Mat< double > * cp_x , const Mat< char > * cp_s , const double clamda , const size_t cm );
    virtual ~BSF();
    bool boost( void );
private:
    size_t m;                        //there are m trees in the forest here m must >= 0 so size_t
    double lamda;                    //the λ the tuning paramemter, balances the retrieval quality and computational cost, need to ask the author Zhen Li
    Tree * forestEntrance;           //Entrance of the forest, in fact is a place where trees reside
    const Mat< char > * p_s;         //a pointer to the similarity matrix [ s00 s01 ... s0n ; s10 s11 .. s1n ; .... ; sn0 sn1 ... snn ] the similarity matrix's diagno is 1 and symmetrical
    const Mat< double > * p_x;       //a pointer to the original sample matrix [ x0f0 x0f1 x0f2 .. x0fk ; x1f0 x1f1 x1f2 ..x1fk ; ... ; xnf0 xnf1 xnf2 ... xnfk ]
    Mat< double > * p_w;             //weight vector [w00 w01 w02 ...; w10 w11 w12..... wn0 wn1 .. 2nn ]


};

#endif	/* BSF_H */

