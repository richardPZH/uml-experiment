#include "StoreRowVector.h"

bool StoreRowVector:: insert(int& item)
{
    vt.push_back( item );
}

bool StoreRowVector:: search(int& item)
{
    //apply the binary search This vector is assume that the row in database is already sorted!
//    vector<int> ::iterator l,r,mid;
//    l = vt.begin();
    int l,r,mid;
    
    l=0;
    r=vt.size()-1;
    
    while( l <= r )
    {
        mid = ( l + r )/2;
        
        if( vt[mid] == item )
            return true;
        else if( vt[mid] > item )
            r = mid - 1;
        else
            l = mid + 1;
    }
    
    return false;
}

bool StoreRowVector:: clear( void )
{
    vt.clear();
}
