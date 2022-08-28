/*
 * File            : aufs.c
 * Author          : ZhangLe
 * CreateTime      : 2022-08-27 19:35:19
 * LastModified    : 2022-08-28 18:12:22
 * Vim             : ts=4, sw=4
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

static struct file_system_type au_fs_type = {
    .owner = THIS_MODULE,
    .name = "aufs",
};

static struct vfsmount *aufs_mount;
//static int aufs_mount_count;

static int __init aufs_init(void)
{
    int retval;

    printk(KERN_INFO "Zhongyl INIT AUFS!\n");

    retval = register_filesystem(&au_fs_type);

    //if(!retval)
    //{
    //    printk(KERN_INFO "Zhongyl REGISTER AUFS\n");
    //    //aufs_mount = kern_mount(&au_fs_type);
    //    //if(IS_ERR(aufs_mount))
    //    //{
    //    //    printk(KERN_ERR "Zhongyl aufs: count not mount!\n");
    //    //}
    //}

    return 0;
}

static void __exit aufs_exit(void)
{
    printk(KERN_INFO "Zhongyl EXIT AUFS!\n");

    unregister_filesystem(&au_fs_type);
    return;
}

module_init(aufs_init);
module_exit(aufs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AUFS");
