/* 
 * File:   DBreaderImp.h
 * Author: IMS@SCUT
 *
 * Created on 2012年5月22日, 下午4:34
 */

#ifndef DBREADERIMP_H
#define	DBREADERIMP_H

#include<cstdlib>
#include<iostream>
#include<fstream>
#include"DBreader.h"
#include"StoreRow.h"
#include"StoreRowVector.h"

using namespace std;

class DBreaderImp : public DBreader
{
public:
    DBreaderImp( char * fileName )
    {
        ifile.open(fileName,ios::in);
        if( ! ifile )
        {
            cerr<<"Error open file "<<__FILE__<<" "<<__LINE__<<endl;
            exit( 1 );
        }
        
        p_sr= new StoreRowVector();
        rowNum=0;
    }
    
    ~DBreaderImp(){ delete p_sr; }
    
    bool readOneRow( void );
    bool moveToFront( void );
    bool readOneItem( int & item );
    bool search( int & item );
    
private:
    StoreRow * p_sr;
    int rowNum;
    ifstream ifile;
};


#endif	/* DBREADERIMP_H */

