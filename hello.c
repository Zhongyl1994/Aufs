/*
 * File            : inode.c
 * Author          : ZhangLe
 * CreateTime      : 2022-08-30 21:13:08
 * LastModified    : 2022-08-31 10:07:43
 * Vim             : ts=4, sw=4
 */

#include "hello.h"
#include <linux/slab.h>
#include <linux/module.h>

static struct kmem_cache * hello_inode_cachep;

static void init_once(void *foo)
{
    struct hello_inode_info *ei = (struct hello_inode_info *) foo;

    inode_init_once(&ei->vfs_inode);
}

static int init_inodecache(void)
{
    hello_inode_cachep = kmem_cache_create("hello_inode_cache",
            sizeof(struct hello_inode_info),
            0, (SLAB_RECLAIM_ACCOUNT|
                SLAB_MEM_SPREAD|SLAB_ACCOUNT),
            init_once);
    if(hello_inode_cachep == NULL)
        return -ENOMEM;
    return 0;
}

static struct file_system_type hello_fs_type = {
    .owner = THIS_MODULE,
    .name = "hello",
};

static int __init init_hello_fs(void)
{
    int err = init_inodecache();

    printk(KERN_ERR "Hellofs: init_hello_fs!\n");

    if (err)
        goto out1;
    return 0;
out1:
    return err;
}

static void __exit exit_hello_fs(void)
{
    unregister_filesystem(&hello_fs_type);
}

module_init(init_hello_fs);
module_exit(exit_hello_fs);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HELLO");
