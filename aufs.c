/*
 * File            : aufs.c
 * Author          : ZhangLe
 * CreateTime      : 2022-08-27 19:35:19
 * LastModified    : 2022-08-30 11:10:54
 * Vim             : ts=4, sw=4
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mount.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/namei.h>

#define AUFS_MAGIC	0x64668735

static struct vfsmount *aufs_vfs_mount;
static int aufs_mount_count;

static int aufs_fill_super(struct super_block *sb, void *data, int silent)
{
    static struct tree_descr debug_files[] = {{""}};
    printk(KERN_ERR "Zhongyl aufs: fill_super!\n");

    return simple_fill_super(sb, AUFS_MAGIC, debug_files);     // Zhongyl, create root inode and root dentry
}

static struct dentry* aufs_mount(struct file_system_type *fs_type,
        int flags, void *data)
{
    struct dentry* root;

    printk(KERN_ERR "Zhongyl aufs: aufs_mount!\n");
    root = mount_single(fs_type, flags, data, aufs_fill_super);
    return root;
}

static struct file_system_type au_fs_type = {
    .owner = THIS_MODULE,
    .name = "aufs",
    .mount = aufs_mount,
    .kill_sb = kill_litter_super,
};

static struct inode *aufs_get_inode(struct super_block* sb, int mode, dev_t dev){
    struct inode* inode = new_inode(sb);

    if(inode){
        inode->i_mode = mode;
        inode->i_uid = current->cred->fsuid;
        inode->i_gid = current->cred->fsgid;
        inode->i_size = PAGE_SIZE;
        inode->i_blocks = 0;
        inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
        switch(mode & S_IFMT){
            default:
                init_special_inode(inode, mode, dev);
                break;
            case S_IFREG:
                printk("Zhongyl aufs: create a file\n");
                break;
            case S_IFDIR:
                printk("Zhongyl aufs: create a dir file\n");

                inode->i_op = &simple_dir_inode_operations;
                inode->i_fop = &simple_dir_operations;
                inc_nlink(inode);
                break;
        }
    }
    return inode;
}

static int aufs_mknod(struct inode* dir, struct dentry* dentry, int mode,
        dev_t dev)
{
    struct inode *inode;
    int error = -EPERM;

    if(dentry->d_inode){
        return -EEXIST;
    }

    inode = aufs_get_inode(dir->i_sb, mode, dev);

    if(inode){
        d_instantiate(dentry, inode);
        dget(dentry);
        error = 0;
    }

    return error;
}

static int aufs_mkdir(struct inode* dir, struct dentry* dentry, int mode){
    int res;

    res = aufs_mknod(dir, dentry, mode | S_IFDIR, 0);
    if (!res){
         inc_nlink(dir);
    }
    return res;
}

static int aufs_create(struct inode *dir, struct dentry *dentry, int mode){
    return aufs_mknod(dir, dentry, mode | S_IFREG, 0);
}

static int aufs_create_by_name(const char* name, mode_t mode,
        struct dentry* parent,
        struct dentry** dentry)
{
    int error = 0;
    if(!parent){
        if(aufs_vfs_mount && aufs_vfs_mount->mnt_sb) {
            printk("Zhongyl aufs: parent = root");
            parent = aufs_vfs_mount->mnt_sb->s_root;
        }
    }

    if(!parent){
        printk("Zhongyl aufs: can not find a parent!");
        return -EFAULT;
    }

    *dentry = NULL;

    mutex_lock(&parent->d_inode->i_mutex);

    *dentry = lookup_one_len(name, parent, strlen(name));
    if(!IS_ERR(dentry)){
        if((mode & S_IFMT) == S_IFDIR)
            error = aufs_mkdir(parent->d_inode, *dentry, mode);
        else
            error = aufs_create(parent->d_inode, *dentry, mode);
    }
    else{
        error = PTR_ERR(dentry);
    }

    mutex_unlock(&parent->d_inode->i_mutex);

    return error;
}

struct dentry* aufs_create_file(const char* name, mode_t mode,
        struct dentry *parent, void* data,
        struct file_operations* fops)
{
    struct dentry *dentry = NULL;
    int error;

    printk("Zhongyl aufs:creating file '%s' \n", name);
    error = aufs_create_by_name(name, mode, parent, &dentry);
    if (error) {
        dentry = NULL;
        goto exit;
    }

    if (dentry->d_inode) {
        if(fops)
            dentry->d_inode->i_fop = fops;
    }

exit:
    return dentry;
}

struct dentry* aufs_create_dir(const char* name, struct dentry* parent)
{
    return aufs_create_file(name,
            S_IFDIR | S_IRWXU | S_IRUGO | S_IXUGO,
            parent, NULL, NULL);
}

static int __init aufs_init(void)
{
    int err = register_filesystem(&au_fs_type);
    struct dentry *pslot;

    printk(KERN_INFO "Zhongyl aufs: INIT AUFS!\n");

    if(!err){
        printk(KERN_INFO "Zhongyl aufs: REGISTER AUFS\n");

        struct vfsmount *mnt;
        mnt = kern_mount(&au_fs_type);

        if(IS_ERR(mnt)){
            printk(KERN_ERR "Zhongyl aufs: count not mount!\n");
            unregister_filesystem(&au_fs_type);
            return err;
        } else {
            aufs_vfs_mount = mnt;
        }
    }

    pslot = aufs_create_dir("dir1", NULL);
    aufs_create_file("zhongyl1", S_IFREG | S_IRUGO, pslot, NULL, NULL);
    aufs_create_file("zhongyl2", S_IFREG | S_IRUGO, pslot, NULL, NULL);
    aufs_create_file("zhongyl3", S_IFREG | S_IRUGO, pslot, NULL, NULL);

    pslot = aufs_create_dir("dir2", NULL);
    aufs_create_file("zhongyl4", S_IFREG | S_IRUGO, pslot, NULL, NULL);
    aufs_create_file("zhongyl5", S_IFREG | S_IRUGO, pslot, NULL, NULL);
    aufs_create_file("zhongyl6", S_IFREG | S_IRUGO, pslot, NULL, NULL);
    return err;
}

static void __exit aufs_exit(void)
{
    printk(KERN_INFO "Zhongyl EXIT AUFS!\n");

    simple_release_fs(&aufs_vfs_mount, &aufs_mount_count);
    unregister_filesystem(&au_fs_type);
    return;
}

module_init(aufs_init);
module_exit(aufs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AUFS");
