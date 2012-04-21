/*
 * 2012/2/7 IMS@SCUT nemosail@gmail.com
 * Pretty cold and wet
 *
 * Directory.h
 * Directories should be also treated as a file. Each directory contains a list of
 * u_fs_directory_entry structures. There is no limit on how many directories we can have.
 *
*/

#ifndef IMS_DIRECTORY_H
#define IMS_DIRECTORY_H

#include "Global.h"
#include <sys/types.h>              //for size_t


struct u_fs_file_directory_entry  //This name is better, I think.
{
    char fname[MAX_FILENAME + MAX_EXTENSION + 1]; //filename (plus space for nul) general filename. it makes life easier
    char fext[MAX_EXTENSION + 1]; //extension (plus space for nul)
    size_t fsize;                 //file size
    long nStartBlock;             //where the first block is on disk
    int flag;                     //indicate type of file. 0:for unused; 1:for file; 2:for directory
};


#endif
