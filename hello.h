/*
 * File            : hello.h
 * Author          : ZhangLe
 * CreateTime      : 2022-08-30 21:17:09
 * LastModified    : 2022-08-31 10:04:22
 * Vim             : ts=4, sw=4
 */

#ifndef _HELLO_H_
#define _HELLO_H_

#include <linux/fs.h>

/*
 * hello fs inode data in memory
 */

struct hello_inode_info {
    union{
        __u16 i1_data[16];
        __u32 i2_data[16];
    } u;
    struct inode vfs_inode;
};

#endif    /* hello.h */

