/*
 * File            : inode.c
 * Author          : ZhangLe
 * CreateTime      : 2022-08-30 21:13:08
 * LastModified    : 2022-08-31 10:42:54
 * Vim             : ts=4, sw=4
 */

#include <linux/module.h>
#include "hello.h"
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/highuid.h>
#include <linux/vfs.h>
#include <linux/writeback.h>

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

static void destroy_inodecache(void)
{
    /*
     * Make sure all delayed rcu free inodes are fulshed before
     * we destory cache.
     */

    rcu_barrier();
    kmem_cache_destroy(hello_inode_cachep);
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
    err = register_filesystem(&hello_fs_type);
    if (err)
        goto out;
    return 0;
out:
    destroy_inodecache();
out1:
    return err;
}

static void __exit exit_hello_fs(void)
{
    unregister_filesystem(&hello_fs_type);
    destroy_inodecache();
}

module_init(init_hello_fs);
module_exit(exit_hello_fs);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HELLO");
