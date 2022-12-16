#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/mmu_context.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/rwsem.h>
#include <linux/mm_types.h>
#include <asm/uaccess.h>
#include <linux/unistd.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artem Bachurinskiy");
MODULE_DESCRIPTION("LKM for vm_area_struct, dentry");
MODULE_VERSION("0.01");

struct dentry *lkm_dir;
#define ker_buf_len 300
char ker_buf[ker_buf_len];

struct mutex mutex;
static struct dentry *pid1_task_struct;
static struct debugfs_blob_wrapper blob_wrapper;
static struct task_struct *ts1;

struct vm_area_struct_debug {
        /* The first cache line has the info for VMA tree walking. */

        unsigned long vm_start;		/* Our start address within vm_mm. */
        unsigned long vm_end;		/* The first byte after our end address
                                           within vm_mm. */

        /* linked list of VM areas per task, sorted by address */
        struct vm_area_struct *vm_next, *vm_prev;

        struct rb_node vm_rb;


//        /*
//         * Largest free memory gap in bytes to the left of this VMA.
//         * Either between this VMA and vma->vm_prev, or between one of the
//         * VMAs below us in the VMA rbtree and its ->vm_prev. This helps
//         * get_unmapped_area find a free area of the right size.
//         */
        unsigned long rb_subtree_gap;

//        /* Second cache line starts here. */

        //struct mm_struct *vm_mm;	/* The address space we belong to. */
        //pgprot_t vm_page_prot;		/* Access permissions of this VMA. */
        //unsigned long vm_flags;		/* Flags, see mm.h. */

//        /*
//         * For areas with an address space and backing store,
//         * linkage into the address_space->i_mmap interval tree, or
//         * linkage of vma in the address_space->i_mmap_nonlinear list.
//         */
//        union {
//                struct {
//                        struct rb_node rb;
//                        unsigned long rb_subtree_last;
//                } linear;
//                struct list_head nonlinear;
//        } shared;

//        /*
//         * A file's MAP_PRIVATE vma can be in both i_mmap tree and anon_vma
//         * list, after a COW of one of the file pages.	A MAP_SHARED vma
//         * can only be in the i_mmap tree.  An anonymous MAP_PRIVATE, stack
//         * or brk vma (with NULL file) can only be in an anon_vma list.
//         */
//        struct list_head anon_vma_chain; /* Serialized by mmap_sem &
//                                          * page_table_lock */
//        struct anon_vma *anon_vma;	/* Serialized by page_table_lock */

//        /* Function pointers to deal with this struct. */
        //const struct vm_operations_struct *vm_ops;

//        /* Information about our backing store: */
//        unsigned long vm_pgoff;		/* Offset (within vm_file) in PAGE_SIZE
//                                           units, *not* PAGE_CACHE_SIZE */
//        struct file * vm_file;		/* File we map to (can be NULL). */
//        void * vm_private_data;		/* was vm_pte (shared mem) */

//#ifndef CONFIG_MMU
//        struct vm_region *vm_region;	/* NOMMU mapping region */
//#endif
//#ifdef CONFIG_NUMA
//        struct mempolicy *vm_policy;	/* NUMA policy for the VMA */
//#endif
};

struct vm_area_struct_wrap {
    struct vm_area_struct_debug vma;
    void *vma_ptr;
};

#define MAX_VM_AREA_NUMBER 256
struct TEST_LKM_VM_AREA {
    uint32_t pid;
    uint32_t pid_found;
    uint32_t vw_area_number;
    struct vm_area_struct_wrap vm_area_list[MAX_VM_AREA_NUMBER];

};

struct TEST_LKM_VM_AREA *test_lkm_vm_area;

// dentry (based on file)
struct TEST_LKM_DENTRY {
    char filename[ker_buf_len];
    struct dentry d;
};
struct TEST_LKM_DENTRY test_lkm_dentry;

/* write file operation */
static ssize_t pid_write_handler(struct file *fp, const char __user *user_buffer, size_t count, loff_t *position)
{
        struct mm_struct *mm;
        struct vm_area_struct *cur_mmap;
        struct vm_area_struct_debug *cur_mmap_debug;
        u32 pid_value;
        int rc;

        mutex_lock(&mutex);

        if(count > ker_buf_len )
        {
            mutex_unlock(&mutex);
            return -EINVAL;
        }

        rc=simple_write_to_buffer(ker_buf, ker_buf_len, position, user_buffer, count);
        if( rc < 0)
        {
            mutex_unlock(&mutex);
            return rc;
        }

        test_lkm_vm_area=kmalloc(sizeof(struct TEST_LKM_VM_AREA),GFP_KERNEL);
        if(!test_lkm_vm_area)
        {
            mutex_unlock(&mutex);
            return -EAGAIN;//no memory
        }


        pid_value=*((u32 *)ker_buf);

        test_lkm_vm_area->pid=pid_value;
        test_lkm_vm_area->pid_found=0;
        test_lkm_vm_area->vw_area_number=0;

        ts1 = get_pid_task(find_get_pid(pid_value), PIDTYPE_PID);
        if(ts1)
        {
            test_lkm_vm_area->pid_found=1;

            mm=get_task_mm(ts1);
            if(mm)
            {
                cur_mmap=mm->mmap;
                test_lkm_vm_area->vw_area_number=0;
                while(cur_mmap)
                {
                    //test_lkm_vm_area.vm_area_list[test_lkm_vm_area.vw_area_number].vma=*cur_mmap;

                    cur_mmap_debug=&test_lkm_vm_area->vm_area_list[test_lkm_vm_area->vw_area_number].vma;
                    cur_mmap_debug->vm_start=cur_mmap->vm_start;
                    cur_mmap_debug->vm_end=cur_mmap->vm_end;
                    cur_mmap_debug->vm_next=cur_mmap->vm_next;
                    cur_mmap_debug->vm_prev=cur_mmap->vm_prev;
                    cur_mmap_debug->vm_rb=cur_mmap->vm_rb;
                    cur_mmap_debug->rb_subtree_gap=cur_mmap->rb_subtree_gap;


                    test_lkm_vm_area->vm_area_list[test_lkm_vm_area->vw_area_number].vma_ptr=cur_mmap;
                    test_lkm_vm_area->vw_area_number++;
                    if(test_lkm_vm_area->vw_area_number>=MAX_VM_AREA_NUMBER)
                        break;

                    cur_mmap=cur_mmap->vm_next;
                }

            }
            else
                printk(KERN_INFO "get_task_mm with given task fails\n");
        }
        else
            printk(KERN_INFO "task with given pid not found\n");


        blob_wrapper.data = (void*)test_lkm_vm_area;
        blob_wrapper.size = sizeof(struct TEST_LKM_VM_AREA);

        pid1_task_struct = debugfs_create_blob("blob_vm_area", 0644, lkm_dir, &blob_wrapper);
        if (pid1_task_struct == NULL)
        {
            kfree(test_lkm_vm_area);
            mutex_unlock(&mutex);

            printk(KERN_INFO "error writing file: blob_vm_area\n");
            return -EINVAL;
        }

        kfree(test_lkm_vm_area);
        mutex_unlock(&mutex);

        return count;

}
static const struct file_operations fops_pid = {
        .write = pid_write_handler,
};

/* write file operation */
static ssize_t dentry_write_handler(struct file *fp, const char __user *user_buffer, size_t count, loff_t *position)
{
        struct dentry *dptr;
        int rc;

        mutex_lock(&mutex);

        if(count > ker_buf_len )
        {
            mutex_unlock(&mutex);
            return -EINVAL;
        }

        rc=simple_write_to_buffer(ker_buf, ker_buf_len, position, user_buffer, count);
        if( rc < 0)
        {
            mutex_unlock(&mutex);
            return rc;
        }

        memcpy(test_lkm_dentry.filename,ker_buf,count);

        // create an example file with specified name to get its dentry struct
        dptr = debugfs_create_file(test_lkm_dentry.filename, 0466, lkm_dir, &test_lkm_dentry, &fops_pid);
        if(dptr!=NULL){

            memcpy(&test_lkm_dentry.d,dptr,sizeof(struct dentry));
            debugfs_remove(dptr);
        }

        //DEBUG test_lkm_dentry.d.d_seq.sequence=sizeof(struct list_head);//struct rb_node


        blob_wrapper.data = (void*)&test_lkm_dentry;
        blob_wrapper.size = sizeof(struct TEST_LKM_DENTRY);

        pid1_task_struct = debugfs_create_blob("blob_dentry", 0644, lkm_dir, &blob_wrapper);
        if (pid1_task_struct == NULL)
        {
            printk(KERN_INFO "error writing file: blob_dentry\n");

            mutex_unlock(&mutex);

            return -EINVAL;
        }

    mutex_unlock(&mutex);
    return count;

}
static const struct file_operations fops_dentry = {
        .write = dentry_write_handler,
};

static int __init lkm_example_init(void) {
        struct dentry *fileret;
 
        /* create a directory by the name dell in sys-kernel-debugfs */
        lkm_dir = debugfs_create_dir("lkm_debug2", NULL);
        if(lkm_dir==NULL){
                printk("error creating dir \n");
                return (-ENOENT);
        }
        if(lkm_dir==ERR_PTR(-ENODEV)){
                printk(KERN_INFO "error creating dir -ENODEV \n");
                return (-ENODEV);
        }



        /* for PID: create a file in the above directory. This requires write file operations */
        fileret = debugfs_create_file("pid", 0466, lkm_dir, &test_lkm_vm_area, &fops_pid);
        if(fileret==NULL){
                printk("error creating file \n");
                return (-ENOENT);
        }
        if(fileret==ERR_PTR(-ENODEV)){
                printk("error creating file -ENODEV \n");
                return (-ENODEV);
        }



        /* for DENTRY: create a file in the above directory. This requires write file operations */
        fileret = debugfs_create_file("filename", 0466, lkm_dir, &test_lkm_dentry, &fops_dentry);
        if(fileret==NULL){
                printk("error creating file \n");
                return (-ENOENT);
        }
        if(fileret==ERR_PTR(-ENODEV)){
                printk("error creating file -ENODEV \n");
                return (-ENODEV);
        }

        mutex_init(&mutex);

        printk(KERN_INFO "KLM ARTEM mounted!\n");
        return 0;
}

static void __exit lkm_example_exit(void) {

	/* removing the directory recursively which in turn cleans all the file */
        debugfs_remove_recursive(lkm_dir);

	printk(KERN_INFO "KLM ARTEM dismounted!\n");
}
module_init(lkm_example_init);
module_exit(lkm_example_exit);
