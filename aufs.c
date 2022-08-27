/*
 * File            : aufs.c
 * Author          : ZhangLe
 * CreateTime      : 2022-08-27 19:35:19
 * LastModified    : 2022-08-27 21:06:48
 * Vim             : ts=4, sw=4
 */

//static struct file_system_type au_fs_type = {
//    .owner = THIS_MODULE,
//    .name = "zhongylfs",
//    .get_sb = aufs_get_sb,
//    .kill_sb = kill_litter_super,
//};
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init aufs_init(void)
{
    //int retval;
    //struct dentry *pslot;

    //retval = register_filesystem(&au_fs_type);
    printk(KERN_INFO "Zhongyl INIT AUFS\n");
    return 0;
}

static void __exit aufs_exit(void)
{
    printk(KERN_INFO "Zhongyl EXIT AUFS!\n");
    return;
}

module_init(aufs_init);
module_exit(aufs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AUFS");
