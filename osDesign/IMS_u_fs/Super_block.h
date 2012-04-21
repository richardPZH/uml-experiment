/*
 * 2012/2/7 IMS@SCUT nemosail@gmail.com
 * Pretty cold and wet
 * Super_block.h
 *
 * This header is used to describe the Supter block in the u_fs FUSE File System
 *
 * Super block
 * Super block must be the first block of the file system. It descripts the whole file system. Info
*/

#ifndef IMS_SUPER_BLOCK_H
#define IMS_SUPER_BLOCK_H

#include "Global.h"

struct sb{
    long fs_size;   //size of file system,in blocks
    long us_size;   //size of used, in blocks
    long bk_size;   //size of the block, in byte
    long first_blk; //first block of root directory
    long bitmap;    //size of bitmap, in blocks
                    //may need more information here
};

#endif
