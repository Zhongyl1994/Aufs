/*
 * File            : aufs.c
 * Author          : ZhangLe
 * CreateTime      : 2022-08-27 19:35:19
 * LastModified    : 2022-08-28 17:07:18
 * Vim             : ts=4, sw=4
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static struct file_system_type au_fs_type = {
    .name = "aufs",
};

static int __init aufs_init(void)
{
    printk(KERN_INFO "Zhongyl INIT AUFS\n");

    int retval;
    retval = register_filesystem(&au_fs_type);
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
