/*
 * 2012/2/7 IMS@SCUT nemosail@gmail.com
 * Global.h
 *
 * This file contains global settings, marco, defines...
 * Almost every file shall contain this file to have the idential settings.
 *
*/

#ifndef IMS_GLOBAL_H
#define IMS_GLOBAL_H

#include <sys/types.h>

//File system related
#define BLOCK_SIZE 512        //in byte
#define SUPER_BLOCK_N 1       //in block
#define BITMAP_BLOCK_N 1280   //in block
#define DATA_BLOCK_N 8959     //in block

//Directory related
#define MAX_FILENAME 64 
#define MAX_EXTENSION 3
#define iUNSUED 0
#define iFILE   1
#define iDIRECTORY 2

//Files related
#define MAX_DATA_IN_BLOCK (BLOCK_SIZE - sizeof( size_t ) - sizeof( long ))


#endif
