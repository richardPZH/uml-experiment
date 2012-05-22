/* 
 * File:   StoreRow.h
 * Author: IMS@SCUT
 *
 * Created on 2012年5月22日, 下午4:36
 */

#ifndef STOREROW_H
#define	STOREROW_H

class StoreRow
{
public:
    virtual bool insert( int & item )=0;
    virtual bool search( int & item )=0;
    virtual bool clear( void )=0;
};


#endif	/* STOREROW_H */

