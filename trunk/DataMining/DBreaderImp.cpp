#include"DBreaderImp.h"

bool DBreaderImp:: readOneRow( void )
{
    int numOfitem , tmp;
    
    p_sr->clear();  //empty capisitor
    
    if( ifile >> numOfitem )
    {
        for( int i=0; i<numOfitem; i++ )
        {
            ifile>>tmp;
            p_sr->insert( tmp );
        }
        
        tRow++; 
        return true;
    }
    else  //read end of file
    {
        return false;        
    }
    
    return false;  //keep the compiler happpy and safer?
}

bool DBreaderImp:: readOneItem(int& item)
{
    if( 0 == rowNum )    
    {
        if( ! (ifile>>rowNum ))
        {
            //EOF reached
            return false;                
        }
        
        tRow++;
    }
    
    ifile>>item;
    rowNum--;
    return true;
}

bool DBreaderImp:: moveToFront( void )
{
    return ifile.seekg( 0 );
}

bool DBreaderImp:: search(int& item)
{
    return p_sr->search( item );
}

int DBreaderImp:: totalRow( void )
{
    return tRow;
}
