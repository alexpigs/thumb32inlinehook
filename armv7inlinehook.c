/**
author: zxe
email:alexpigs@qq.com
*/

#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mman.h>
#include "armv7inlinehook.h"

static void android_clear_cache(unsigned char* begin, unsigned char *end)
{
	register const unsigned char *r0 asm("r0") = begin;
	register const unsigned char *r1 asm("r1") = end;
	register const int r2 asm("r2") = 0;
	register const int r7 asm("r7") = 0xf0002;
	asm volatile ("svc 0x0" :: "r" (r0), "r" (r1), "r" (r2), "r" (r7));
}


int armv7_inline_hook(void *target, void *tramp, void *fake){
	unsigned char n_code[HIJACK_SIZE];
	size_t pagesize = sysconf(_SC_PAGESIZE);
	mprotect((void *)(((size_t)target) & (((size_t)-1) ^ (pagesize - 1))), pagesize, PROT_EXEC| PROT_READ | PROT_WRITE);
	mprotect((void *)(((size_t)tramp) & (((size_t)-1) ^ (pagesize - 1))), pagesize, PROT_EXEC| PROT_READ | PROT_WRITE);

	*(unsigned long *)(tramp + 0x4) = (unsigned long)fake;
	*(unsigned long *)(tramp + 0x8) = (unsigned long)fake;
	memcpy(tramp+0x10, target, HIJACK_SIZE);
	*(unsigned long *)(tramp + 0x20) = (unsigned long)target + HIJACK_SIZE;
	*(unsigned long *)(tramp + 0x24) = (unsigned long)target + HIJACK_SIZE;

	// ldr pc, [pc, #0]; .long addr; .long addr
	memcpy(target, "\x00\xf0\x9f\xe5\x00\x00\x00\x00\x00\x00\x00\x00", HIJACK_SIZE);
	*(unsigned long *)(target+4) = (unsigned long)tramp;
	*(unsigned long *)(target+8) = (unsigned long)tramp;
	android_clear_cache(target,target+HIJACK_SIZE);
	android_clear_cache(tramp,tramp+0x30);
	return 0;
}
