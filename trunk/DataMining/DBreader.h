/* 
 * File:   DBreader.h
 * Author: IMS@SCUT
 *
 * Created on 2012年5月22日, 下午4:29
 */

#ifndef DBREADER_H
#define	DBREADER_H

//This is an interface like class
class DBreader
{
public:
    virtual bool readOneRow( void )=0;
    virtual bool moveToFront( void )=0;
    virtual bool readOneItem( int & item )=0;
    virtual bool search( int & item )=0;
    virtual int totalRow( void )=0;
    virtual ~DBreader(){};
};


#endif	/* DBREADER_H */

