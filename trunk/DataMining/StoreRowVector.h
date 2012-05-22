/* 
 * File:   StoreRowVector.h
 * Author: IMS@SCUT
 *
 * Created on 2012年5月22日, 下午4:40
 */

#ifndef STOREROWVECTOR_H
#define	STOREROWVECTOR_H

#include<vector>

using namespace std;

class StoreRowVector :public StoreRow
{
public:
    bool insert( int & item );
    bool search( int & item );
    
private:
    vector<int> vt;
};


#endif	/* STOREROWVECTOR_H */

