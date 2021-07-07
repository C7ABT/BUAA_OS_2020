// implement fork from user space

#include "lib.h"
#include <mmu.h>
#include <env.h>


/* ----------------- help functions ---------------- */

/* Overview:
 *      Copy `len` bytes from `src` to `dst`.
 *
 * Pre-Condition:
 *      `src` and `dst` can't be NULL. Also, the `src` area
 *       shouldn't overlap the `dest`, otherwise the behavior of this
 *       function is undefined.
 */
void user_bcopy(const void *src, void *dst, size_t len)
{
        void *max;

        //      writef("~~~~~~~~~~~~~~~~ src:%x dst:%x len:%xn",(int)src,(int)dst,len);
        max = dst + len;

        // copy machine words while possible
        if (((int)src % 4 == 0) && ((int)dst % 4 == 0)) {
                while (dst + 3 < max) {
                        *(int *)dst = *(int *)src;
                        dst += 4;
                        src += 4;
                }
        }

        // finish remaining 0-3 bytes
        while (dst < max) {
                *(char *)dst = *(char *)src;
                dst += 1;
                src += 1;
        }

        //for(;;);
}

/* Overview:
 *      Sets the first n bytes of the block of memory
 * pointed by `v` to zero.
 *
 * Pre-Condition:
 *      `v` must be valid.
 *
 * Post-Condition:
 *      the content of the space(from `v` to `v`+ n)
 * will be set to zero.
 */
void user_bzero(void *v, u_int n)
{
        char *p;
        int m;

        p = v;
        m = n;

        while (--m >= 0) {
                *p++ = 0;
        }
}
/*--------------------------------------------------------------*/

/* Overview:
 *      Custom page fault handler - if faulting page is copy-on-write,
 * map in our own private writable copy.
 *
 * Pre-Condition:
 *      `va` is the address which leads to a TLBS exception.
 *
 * Post-Condition:
 *  Launch a user_panic if `va` is not a copy-on-write page.
 * Otherwise, this handler should map a private writable copy of
 * the faulting page at correct address.
 */
static void
pgfault(u_int va)
{
    u_int *tmp;

    //map the new page at a temporary place
    tmp = UTEXT - BY2PG;
    va = ROUNDDOWN(va, BY2PG);
    //copy the content
    if(!((*vpt)[VPN(va)] & PTE_COW)){
        user_panic("You are not a fucking COW\n");
    }
    //map the page on the appropriate place
    syscall_mem_alloc(0, tmp, (PTE_V|PTE_R));
    user_bcopy((void*)va, (void*)tmp, BY2PG);
    syscall_mem_map(0, tmp, 0, va, (PTE_V|PTE_R));
    //unmap the temporary place
    syscall_mem_unmap(0, tmp);
}

/* Overview:
 *      Map our virtual page `pn` (address pn*BY2PG) into the target `envid`
 * at the same virtual address.
 *
 * Post-Condition:
 *  if the page is writable or copy-on-write, the new mapping must be
 * created copy on write and then our mapping must be marked
 * copy on write as well. In another word, both of the new mapping and
 * our mapping should be copy-on-write if the page is writable or
 * copy-on-write.
 *
 * Hint:
 *      PTE_LIBRARY indicates that the page is shared between processes.
 * A page with PTE_LIBRARY may have PTE_R at the same time. You
 * should process it correctly.
 */
static void
duppage(u_int envid, u_int pn)
{
    u_int addr;
    u_int perm;
    addr = pn * BY2PG;
    perm = (*vpt)[pn] & 0xfff;
    if((perm & PTE_V) && (perm & PTE_R) && (!(perm & PTE_LIBRARY))){
        syscall_mem_map(0, addr, envid, addr, (perm|PTE_COW));
        syscall_mem_map(0, addr, 0, addr, (perm|PTE_COW));
    } else if(!(perm & PTE_V)) {
        return;
    } else {
        syscall_mem_map(0, addr, envid, addr, perm);
    }
}

/* Overview:
 *      User-level fork. Create a child and then copy our address space
 * and page fault handler setup to the child.
 *
 * Hint: use vpd, vpt, and duppage.
 * Hint: remember to fix "env" in the child process!
 * Note: `set_pgfault_handler`(user/pgfault.c) is different from
 *       `syscall_set_pgfault_handler`.
 */
extern void __asm_pgfault_handler(void);
int
fork(void)
{
    // Your code here.
    u_int newenvid;
    extern struct Env *envs;
    extern struct Env *env;
    u_int i;
    set_pgfault_handler(pgfault);
    newenvid = syscall_env_alloc();
    if(!newenvid){
        env = &envs[ENVX(syscall_getenvid())];
    } else {
        for(i = 0; i < USTACKTOP; i += BY2PG){
            if (!(*vpd)[PDX(i)]) {
                i += PDMAP;
                i -= BY2PG;
                continue;
            }
            duppage(newenvid,VPN(i));
        }
    syscall_mem_alloc(newenvid, (UXSTACKTOP - BY2PG), (PTE_V|PTE_R));
    syscall_set_pgfault_handler(newenvid, __asm_pgfault_handler, UXSTACKTOP);
    syscall_set_env_status(newenvid, ENV_RUNNABLE);
    }

    return newenvid;
}

// Challenge!
int
sfork(void)
{
        user_panic("sfork not implemented");
        return -E_INVAL;
}

