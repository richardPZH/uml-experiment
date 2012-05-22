/* 
 * File:   StoreRowSet.h
 * Author: IMS@SCUT
 *
 * Created on 2012年5月22日, 下午5:21
 */

#ifndef STOREROWSET_H
#define	STOREROWSET_H

#include <set>
#include "StoreRow.h"

using namespace std;

class StoreRowSet : public StoreRow
{
public:
    bool insert( int & item )=0;
    bool search( int & item )=0; 
    
private:
    set<int> st;
};


#endif	/* STOREROWSET_H */

