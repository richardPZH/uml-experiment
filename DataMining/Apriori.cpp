#include <iostream>
#include "Apriori.h"
#include "DBreader.h"

#define MAX_LENGTH 110

using namespace std;

void  Apriori :: generate( DBreader * p_dbr , const double min_support_rate )
{
    //Here We Go, IMS.
    vector<int> inItem( MAX_LENGTH , 0 );
    
    int tmp;
    while( p_dbr->readOneItem( tmp ) )
    {
        inItem[tmp]++;
    }
    
    int support_count =( p_dbr->totalRow() ) * min_support_rate;
    
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
        
        fpt.avt.clear();
    }
    
    list< FreqPattCnt > * temp;
    temp = p;
    p = c;     
    c = temp;              // p now is one-frequent pattern; c now is ready two-frequent
    
    list< FreqPattCnt > ::iterator f,e,t;
    
    f = p->begin();
    e = p->end();
    
    for( ; f != e ; f++ )
    {
        t = f;
        t ++;
         
        for( ; t != e ; t++ )
        {
            fpt.avt.clear();
            fpt.count = 0;
            
            fpt.avt.push_back( f->avt.at(0) );
            fpt.avt.push_back( t->avt.at(0) );
            
            c->push_back( fpt );
        }   
    }
    
    while( ! ( c->empty()) )
    {
        //output the frequent pattern in p
        f = p->begin();
        e = p->end();
        
        while( f != e )
        {
            for( i=0; i < f->avt.size() ; i++ )
            {
                cout<<f->avt.at(i)<<" ";
            }
            cout<<" Support_Count : "<<f->count<<endl;
            
            f++;
        }
        //output done
        
        p_dbr->moveToFront();
        
        while( p_dbr->readOneRow() )
        {
            f = c->begin();
            e = c->end();
            
            for( ; f != e ; f++ )
            {
                int i;
                for(  i=0 ; i < f->avt.size() ; i++ )
                {
                    if( ! (p_dbr->search( f->avt.at(i))) )
                    {
                        break;
                    }
                }
                if( i == f->avt.size() )
                {
                    f->count ++;
                }
            }
        }
        
        f = c->begin();
        e = c->end();
        
        while( f != e )
        {
            if( f->count < support_count )
            {
                f = c->erase( f );
            }
        }
        
        temp = p;
        p = c;
        c = p;
        c->clear();
        
        //use the p(k-1) to generate c(k)
        //remember the cat rule  a. 1 2 4 5 b. 1 2 4 6 -> 1 2 4 5 6
        //in math L(1 .. k-1 ) = L'(1 .. k-1) && L(k) != L'(k)
        f = p->begin();
        e = p->end();
        
        while( f != e )
        {
            t = f;
            t ++;
            
            while( t != e )
            {
                fpt.avt.clear();
                fpt.count = 0;
                
                for( i = 0; i < t->avt.size() - 1; i++ )
                {
                    if( f->avt.at( i ) != t->avt.at(i) )
                    {
                        break;
                    }
             
                }
                
                if( i == f->avt.size() - 1 )
                {
                    for( i=0 ; i < f->avt.size() ; i++ )
                    {
                        fpt.avt.push_back( f->avt.at(i));
                    }
                    
                    fpt.avt.push_back( t->avt.at(i-1));
                    
                    c->push_back( fpt );
                }
            }
        }
        
    }
    
}
