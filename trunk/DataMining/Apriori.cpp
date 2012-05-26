#include "Apriori.h"
#include "DBreader.h"

#define MAX_LENGTH 100

void  Apriori :: generate( DBreader * p_dbr , const double min_support_rate )
{
    //Here We Go, IMS.
    vector<int> inItem( MAX_LENGTH , 0 );
    
    int tmp;
    while( p_dbr->readOneItem( tmp ) )
    {
        inItem.at( tmp ) ++;
    }
    
    int support_count =( int ) ( p_dbr->totalRow() ) * min_support_rate;
    
    FreqPattCnt fpt;
    
    int i;
    for( i=0 ; i<MAX_LENGTH ; i++ )
    {
        if( inItem.at( i ) >= support_count )
        {
            fpt.avt.push_back( i );
            fpt.count = inItem.at( i );
            
            c->push_back( fpt );
        }
    }
    
    
}
