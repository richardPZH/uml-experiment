#include"DBreaderImp.h"

bool DBreaderImp:: readOneRow( void )
{
    int numOfitem , tmp;
    if( ifile >> numOfitem )
    {
        for( int i=0; i<numOfitem; i++ )
        {
            ifile>>tmp;
            p_sr->insert( tmp );
        }
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
