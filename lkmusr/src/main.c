#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>



#include "util.h"

#define FILE_NAME_MAX_LEN 256
char Dentry_FilePathName[FILE_NAME_MAX_LEN]="/sys/kernel/debug/lkm_debug2/filename";
char BlobDentry_FilePathName[FILE_NAME_MAX_LEN]="/sys/kernel/debug/lkm_debug2/blob_dentry";
struct TEST_LKM_DENTRY test_klm_dentry;

char Pid_FilePathName[FILE_NAME_MAX_LEN]="/sys/kernel/debug/lkm_debug2/pid";
char BlobPid_FilePathName[FILE_NAME_MAX_LEN]="/sys/kernel/debug/lkm_debug2/blob_vm_area";
struct TEST_LKM_VM_AREA test_klm_vm_area;



int main(int argc, char **argv) 
{
        int32_t rc,i,pid_value;
        char filename[FILE_NAME_MAX_LEN];

        if ( argc != 3
             ||
             ( ( strcmp(argv[1],"-f") )
               &&
               ( strcmp(argv[1],"-p") )
             )
           )
        {
                printf("arguments format: \n");
                printf("\t[-f <file name>]   -to take dentry struct of a file \n");
                printf("\t[-p <process id>]  -to take vm_area_struct list of the specified process (<process id>=0 for this process)\n\n");
                return 0;
        }



        if( !strcmp(argv[1],"-p") )
        {
            if( !isdigit(*(argv[2])) ||
                (pid_value = atoi(argv[2])) < 0 )
            {
                printf("\nERROR: <process id> should be a positive number or 0\n");
                return 0;
            }

            if(pid_value==0)
                pid_value=getpid();

            if( write_bin_file(Pid_FilePathName,&pid_value,sizeof(uint32_t)) == true )
            {
                printf("PID_VAL: %d\n", pid_value);
            }

            if( read_bin_file(BlobPid_FilePathName,&test_klm_vm_area,sizeof(struct TEST_LKM_VM_AREA)) == true )
            {
                printf("BLOB: pid=%d,pid_found=%d,vw_area_number=%d (max supported %d)\n",
                       test_klm_vm_area.pid,test_klm_vm_area.pid_found,test_klm_vm_area.vw_area_number,MAX_VM_AREA_NUMBER);

                for(i=0;i < test_klm_vm_area.vw_area_number;i++)
                {
                    struct vm_area_struct_debug *vma=&test_klm_vm_area.vm_area_list[i].vma;
                    printf("\nvm_area_list[%d] (kernel address=0x%x): \n",
                           i,(unsigned int)test_klm_vm_area.vm_area_list[i].vma_ptr);

                    printf("\t vm_start=0x%lx\n", (unsigned long)vma->vm_start);
                    printf("\t vm_end=0x%lx\n", (unsigned long)vma->vm_end);
                    printf("\t vm_next=0x%lx\n", (unsigned long)vma->vm_next);
                    printf("\t vm_prev=0x%lx\n", (unsigned long)vma->vm_prev);

                    printf("\t vm_rb.__rb_parent_color=0x%x\n", (unsigned int)vma->vm_rb.__rb_parent_color);
                    printf("\t vm_rb.rb_right=0x%x\n", (unsigned int)vma->vm_rb.rb_right);
                    printf("\t vm_rb.rb_left=0x%x\n", (unsigned int)vma->vm_rb.rb_left);
                    printf("\t rb_subtree_gap=%lu\n", vma->rb_subtree_gap);

//                    printf("\t vm_mm=0x%x\n", (unsigned int)vma->vm_mm);
//                    printf("\t vm_page_prot.pgprot=0x%x\n", (unsigned int)vma->vm_page_prot.pgprot);
//                    printf("\t vm_flags=0x%x\n", (unsigned int)vma->vm_flags);

//                    printf("\t vm_shared.linear.rb.__rb_parent_color=0x%x\n", (unsigned int)vma->shared.linear.rb.__rb_parent_color);
//                    printf("\t shared.linear.rb.rb_right=0x%x\n", (unsigned int)vma->shared.linear.rb.rb_right);
//                    printf("\t shared.linear.rb.rb_left=0x%x\n", (unsigned int)vma->shared.linear.rb.rb_left);
//                    printf("\t shared.linear.rb_subtree_last=0x%lx\n", vma->shared.linear.rb_subtree_last);

//                    printf("\t shared.nonlinear.next=0x%x\n", (unsigned int)vma->shared.nonlinear.next);
//                    printf("\t shared.nonlinear.prev=0x%x\n", (unsigned int)vma->shared.nonlinear.prev);

//                    printf("\t anon_vma_chain.next=0x%x\n", (unsigned int)vma->anon_vma_chain.next);
//                    printf("\t anon_vma_chain.prev=0x%x\n", (unsigned int)vma->anon_vma_chain.prev);

//                    printf("\t vm_ops=0x%x\n", (unsigned int)vma->vm_ops);

//                    printf("\t vm_pgoff=%lu\n", vma->vm_pgoff);

//                    printf("\t vm_file=0x%x\n", (unsigned int)vma->vm_file);
//                    printf("\t vm_private_data=0x%x\n", (unsigned int)vma->vm_private_data);
//                    printf("\t vm_region=0x%x\n", (unsigned int)vma->vm_region);

                }
            }
        }
        else
        if( !strcmp(argv[1],"-f") )
        {
            if(strlen(argv[2]) >= FILE_NAME_MAX_LEN )
            {
                printf("\nERROR: <file name> should not be longer than %d characters \n",FILE_NAME_MAX_LEN);
                return 0;
            }

            strcpy(filename,argv[2]);
            printf("\nstrlen(filename) %d \n",strlen(filename));
            if( write_txt_file(Dentry_FilePathName,filename,strlen(filename)+1) == true )
            {
                printf("filename: %s\n", filename);
            }

            if( read_bin_file(BlobDentry_FilePathName,&test_klm_dentry,sizeof(struct TEST_LKM_DENTRY)) == true )
            {
                printf("DENTRY struct of file: %s\n", test_klm_dentry.filename);
                printf("\t d_flags=0x%x\n", test_klm_dentry.d.d_flags);
                printf("\t d_seq.sequence=%d\n", test_klm_dentry.d.d_seq.sequence);
                printf("\t d_hash.next=0x%x d_hash.pprev=0x%x\n",
                       (unsigned int)test_klm_dentry.d.d_hash.next,(unsigned int)test_klm_dentry.d.d_hash.pprev);
                printf("\t d_parent=0x%x\n", (unsigned int)test_klm_dentry.d.d_parent);
                printf("\t d_name.len=%d\n", test_klm_dentry.d.d_name.len);
                printf("\t d_name.name=0x%x\n", (unsigned int)test_klm_dentry.d.d_name.name);
                //            printf("\t d_name.name=");
                //            for(i=0;i<test_klm_dentry.d.d_name.len;i++)
                //                printf("%c", test_klm_dentry.d.d_name.name[i]);
                //            printf("\n");
                printf("\t d_inode=0x%x\n", (unsigned int)test_klm_dentry.d.d_inode);
                printf("\t d_iname=%s\n", test_klm_dentry.d.d_iname);
                printf("\t d_lockref.count=%d\n", test_klm_dentry.d.d_lockref.count);
                printf("\t d_op=0x%x\n", (unsigned int)test_klm_dentry.d.d_op);
                printf("\t d_sb=0x%x\n", (unsigned int)test_klm_dentry.d.d_sb);
                printf("\t d_time=%lu\n", test_klm_dentry.d.d_time);
                printf("\t d_fsdata=0x%x\n", (unsigned int)test_klm_dentry.d.d_fsdata);
                printf("\t d_lru.next=0x%x d_lru.prev=0x%x\n",
                       (unsigned int)test_klm_dentry.d.d_lru.next,(unsigned int)test_klm_dentry.d.d_lru.prev);
                printf("\t d_child.next=0x%x d_child.prev=0x%x\n",
                       (unsigned int)test_klm_dentry.d.d_child.next,(unsigned int)test_klm_dentry.d.d_child.prev);
                printf("\t d_subdirs.next=0x%x d_subdirs.prev=0x%x\n",
                       (unsigned int)test_klm_dentry.d.d_subdirs.next,(unsigned int)test_klm_dentry.d.d_subdirs.prev);
                printf("\t d_u.d_alias.next=0x%x d_u.d_alias.pprev=0x%x\n",
                       (unsigned int)test_klm_dentry.d.d_u.d_alias.next, (unsigned int)test_klm_dentry.d.d_u.d_alias.pprev);
                printf("\t d_u.d_rcu.func=0x%x d_u.d_rcu.next=0x%x\n",
                       (unsigned int)test_klm_dentry.d.d_u.d_rcu.func, (unsigned int)test_klm_dentry.d.d_u.d_rcu.next);

            }

            //DEBUG printf("sizeof(TEST_LKM_VM_AREA): %d\n", sizeof(struct list_head));//DEBUG struct rb_node
        }
	return 0;
}
