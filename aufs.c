/*
 * File            : aufs.c
 * Author          : ZhangLe
 * CreateTime      : 2022-08-27 19:35:19
 * LastModified    : 2022-08-29 17:44:26
 * Vim             : ts=4, sw=4
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define AUFS_MAGIC	0x64626720

static struct vfsmount *aufs_vfs_mount;
static int aufs_mount_count;

static int aufs_fill_super(struct super_block *sb, void *data, int silent)
{
    static struct tree_descr debug_files[] = {{""}};
    printk(KERN_ERR "Zhongyl aufs: fill_super!\n");

    return simple_fill_super(sb, AUFS_MAGIC, debug_files);     // Zhongyl, create root inode and root dentry
}

struct dentry* aufs_mount(struct file_system_type *fs_type,
        int flags, void *data)
{
    struct dentry* root;

    printk(KERN_ERR "Zhongyl aufs: aufs_mount!\n");
    root = mount_nodev(fs_type, flags, data, aufs_fill_super);
    return root;
}

static struct file_system_type au_fs_type = {
    .owner = THIS_MODULE,
    .name = "aufs",
    .mount = aufs_mount,
};

static int __init aufs_init(void)
{
    int retval;

    printk(KERN_INFO "Zhongyl INIT AUFS!\n");

    retval = register_filesystem(&au_fs_type);

    if(!retval)
    {
        printk(KERN_INFO "Zhongyl REGISTER AUFS\n");
        aufs_vfs_mount = kern_mount(&au_fs_type);

        if(IS_ERR(aufs_vfs_mount))
        {
            printk(KERN_ERR "Zhongyl aufs: count not mount!\n");
            unregister_filesystem(&au_fs_type);
            return retval;
        }
    }

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
