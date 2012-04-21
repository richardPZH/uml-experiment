/*
 * 2012/2/7 IMS@SCUT nemosail@gmail.com
 * Files.h
 *
 * Files
 * Files will be stored in a virtual disk that is implemented as a single, pre-sized file called .disk with 512 byte blocks of the format:
 *
 * This way of management seems to be less efficient the data store can not be 2^n. Because the block is 2^n, but the struct contains number, so it's not 2^n unless half of the block is used for storing data.
*/

#ifndef IMS_FILES_H
#define IMS_FILES_H

#include "Global.h"
#include <sys/types.h>

struct u_fs_disk_block{
    size_t size;      
    //how many bytes are being used in this block
    long nNextBlock;  
    //The next disk block, if needed. This is the next pointer in the linked allocation list
    char data[MAX_DATA_IN_BLOCK];  
    //And all the rest of the space in the block can be used for actual data storage.
};
    





#endif
