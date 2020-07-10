/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

#define IDE_BEGIN_ADDR 0x13000000
#define IDE_OFFSET_ADDR IDE_BEGIN_ADDR + 0x0000
#define IDE_OFFSETHI_ADDR IDE_BEGIN_ADDR + 0x0008
#define IDE_ID_ADDR IDE_BEGIN_ADDR + 0x0010
#define IDE_OP_ADDR IDE_BEGIN_ADDR + 0x0020
#define IDE_STATUS_ADDR IDE_BEGIN_ADDR + 0x0030
#define IDE_BUFFER_ADDR IDE_BEGIN_ADDR + 0x4000
#define IDE_BUFFER_SIZE 0x0200

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
    int temp = 0;
    int flag = 0;
    int offset_current;

	while (offset_begin + offset < offset_end) {
        offset_current = offset_begin + offset;
        
        if ((syscall_write_dev(&diskno, IDE_ID_ADDR, 4)) || (syscall_write_dev(&offset_current, IDE_BEGIN_ADDR, 4)) || 
            (syscall_write_dev(&temp, IDE_OP_ADDR, 4)) || (syscall_read_dev(&flag, IDE_STATUS_ADDR, 4)) || 
            (syscall_read_dev(dst + offset, IDE_BUFFER_ADDR, 512)) 
            )
        {
            user_panic("FGSB\n");
        }
        offset += 512;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
        // Your code here
	int offset_begin = secno * 512;
	int offset_end = offset_begin + nsecs * 512;
	int offset = 0;
    int temp = 1;
    int flag = 0;
    int offset_current;
    
	writef("diskno: %d\n", diskno);
	while (offset_begin + offset < offset_end) {
        offset_current = offset_begin + offset;
        
	    if ((syscall_write_dev(&diskno, IDE_ID_ADDR, 4)) ||(syscall_write_dev(&offset_current, IDE_BEGIN_ADDR, 4)) ||(syscall_write_dev(src + offset, IDE_BUFFER_ADDR, 512)) ||      (syscall_write_dev(&temp, IDE_OP_ADDR, 4)) ||
            (syscall_read_dev(&flag, IDE_STATUS_ADDR, 4))
            )
        {
            user_panic("FGSB\n");
        }
        offset += 512;
	}
}

