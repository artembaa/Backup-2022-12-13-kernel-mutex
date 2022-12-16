#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool write_bin_file(char *file_name,void *buffer,int size);
bool write_txt_file(char *file_name,void *buffer,int size);
bool read_bin_file(char *file_name,void *buffer,int size);
bool read_txt_file(char *file_name,char *buffer,int size);


// LINUX KERNEL VERSION : 3.19.0-80-generic
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;
typedef unsigned short		umode_t;
#ifdef __LITTLE_ENDIAN
 #define HASH_LEN_DECLARE u32 hash; u32 len;
 #define bytemask_from_count(cnt)	(~(~0ul << (cnt)*8))
#else
 #define HASH_LEN_DECLARE u32 len; u32 hash;
 #define bytemask_from_count(cnt)	(~(~0ul >> (cnt)*8))
#endif

typedef struct seqcount {
        unsigned sequence;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
        struct lockdep_map dep_map;
#endif
} seqcount_t;
struct hlist_bl_node {
        struct hlist_bl_node *next, **pprev;
};
typedef unsigned int	__kernel_gid32_t;
typedef __kernel_gid32_t	gid_t;
typedef struct {
        gid_t val;
} kgid_t;

typedef unsigned int __u32;


typedef long long	__kernel_loff_t;
typedef __kernel_loff_t		loff_t;

typedef long		__kernel_long_t;
typedef __kernel_long_t	__kernel_time_t;


typedef struct {
        volatile unsigned int lock;
} arch_spinlock_t;
typedef struct raw_spinlock {
        arch_spinlock_t raw_lock;
#ifdef CONFIG_GENERIC_LOCKBREAK
        unsigned int break_lock;
#endif
#ifdef CONFIG_DEBUG_SPINLOCK
        unsigned int magic, owner_cpu;
        void *owner;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
        struct lockdep_map dep_map;
#endif
} raw_spinlock_t;
typedef struct spinlock {
        union {
                struct raw_spinlock rlock;

#ifdef CONFIG_DEBUG_LOCK_ALLOC
# define LOCK_PADSIZE (offsetof(struct raw_spinlock, dep_map))
                struct {
                        u8 __padding[LOCK_PADSIZE];
                        struct lockdep_map dep_map;
                };
#endif
        };
} spinlock_t;

//typedef u64 blkcnt_t;

typedef struct {
        int counter;
} atomic_t;
struct list_head {
        struct list_head *next, *prev;
};


struct qstr {
        union {
                struct {
                        HASH_LEN_DECLARE;
                };
                u64 hash_len;
        };
        const unsigned char *name;
};
struct hlist_node {
        struct hlist_node *next, **pprev;
};

struct rcu_head {
        struct rcu_head *next;
        void (*func)(struct rcu_head *head);
};


struct lockref {
        union {
#if USE_CMPXCHG_LOCKREF
                aligned_u64 lock_count;
#endif
                struct {
                        spinlock_t lock;
                        unsigned int count;
                };
        };
};


#define DNAME_INLINE_LEN 36 /* 128 bytes */
struct dentry {
        /* RCU lookup touched fields */
        unsigned int d_flags;		/* protected by d_lock */
        seqcount_t d_seq;		/* per dentry seqlock */
        struct hlist_bl_node d_hash;	/* lookup hash list */
        struct dentry *d_parent;	/* parent directory */
        struct qstr d_name;
        void *d_inode;//struct inode *d_inode;		/* Where the name belongs to - NULL is negative */
        unsigned char d_iname[DNAME_INLINE_LEN];	/* small names */
        /* Ref lookup also touches following */
        struct lockref d_lockref;	/* per-dentry lock and refcount */
        const struct dentry_operations *d_op;
        struct super_block *d_sb;	/* The root of the dentry tree */
        unsigned long d_time;		/* used by d_revalidate */
        void *d_fsdata;			/* fs-specific data */

        struct list_head d_lru;		/* LRU list */
        struct list_head d_child;	/* child of parent list */
        struct list_head d_subdirs;	/* our children */
        /*
         * d_alias and d_rcu can share memory
         */
        union {
                struct hlist_node d_alias;	/* inode alias list */
                struct rcu_head d_rcu;
        } d_u;
};



struct rb_node {
        unsigned long  __rb_parent_color;
        struct rb_node *rb_right;
        struct rb_node *rb_left;
} __attribute__((aligned(sizeof(long))));

typedef struct { unsigned long long pgprot; } pgprot_t;

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

// BLOB STRUCTS:
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


#define ker_buf_len 300
struct TEST_LKM_DENTRY {
    char filename[ker_buf_len];
    struct dentry d;
};

#endif



