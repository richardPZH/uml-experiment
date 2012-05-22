#include"StoreRowSet.h"

bool StoreRowSet:: insert(int& item)
{
    return ( st.insert( item )).second;
}

bool StoreRowSet:: search(int& item)
{
    return ( st.find( item ) != st.end() );
}
