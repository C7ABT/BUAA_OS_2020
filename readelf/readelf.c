/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

////////////////////////////////////////////////////////////////////////////

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/

/*
uint16_t reverse_uint16_t(uint16_t x) 
{
	uint16_t answer = 0;
        while(x != 0)
        {
      		answer = answer * 10 + (x % 10);
     		x /= 10;
	}
	return answer;
}
uint32_t reverse_uint32_t(uint32_t x)
{
	uint32_t answer = 0;
	while(x != 0)
	{
		answer = answer * 10 + (x % 10);
		x /= 10;
	}
	return answer;
}
*/
uint32_t reverse_uint32_t(uint32_t v) {
/*
        n=(n>>16)|(n<<16);
        n=((n&0xff00ff00)>>8)|((n&0x00ff00ff)<<8);
        n=((n&0xf0f0f0f0)>>4)|((n&0x0f0f0f0f)<<4);
        n=((n&0xcccccccc)>>2)|((n&0x33333333)<<2);
        n=((n&0xaaaaaaaa)>>1)|((n&0x55555555)<<1);
*/
        return ((v & 0x000000FF) << 24) | ((v & 0x0000FF00) << 8) | ((v & 0x00FF0000) >> 8) | ((v & 0xFF000000) >> 24) ;
}
uint16_t reverse_uint16_t(uint16_t x){
/*
	x = (x & 0x55555555) << 1 | (x >> 1) & 0x55555555;
	x = (x & 0x33333333) << 2 | (x >> 2) & 0x33333333;
	x = (x & 0x0f0f0f0f) << 4 | (x >> 4) & 0x0f0f0f0f;
	x = (x & 0x00ff00ff) << 8 | (x >> 8) & 0x00ff00ff;
	x = (x & 0x0000ffff) << 16 | (x >> 16) & 0x0000ffff;
*/
	return ((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8) ;
}

int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;
	int flag = ehdr->e_ident[5];
        Elf32_Phdr *phdr = NULL;

	u_char *ptr_ph_table = binary + reverse_uint32_t(ehdr->e_phoff);

        Elf32_Half ph_entry_count = ehdr->e_phnum;
        Elf32_Half ph_entry_size = ehdr->e_phentsize;


        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }
	//
	else
	{
		if(flag == 1){
			phdr = (Elf32_Phdr*)(binary + ehdr->e_phoff);
			for(Nr = 0; Nr < ph_entry_count; Nr++, phdr++){
				printf("%d:0x%x,0x%x\n", Nr, phdr->p_filesz, phdr->p_memsz);
			}
		}

		if(flag == 2){
			for(Nr = 0; Nr < reverse_uint16_t(ph_entry_count); Nr++){
				phdr = (Elf32_Phdr*)(ptr_ph_table + Nr * reverse_uint16_t(ph_entry_size));
		       		printf("%d:0x%x,0x%x\n",Nr,reverse_uint32_t(phdr->p_filesz),reverse_uint32_t(phdr->p_memsz));
			}
		}
	
	}
        // hint: section number starts at 0.


        return 0;
}

