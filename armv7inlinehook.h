/**
	author: zxe
	email:alexpigs@qq.com


	usage sample: inline hook ioctl
	define : 

	int fake_ioctl (int __fd, unsigned long int __request, void * arg)
	{
	
	int res = tramp_ioctl(__fd, __request, arg);	
	return res;
	}

TRAMPOLINE_FUNCTION(ioctl, int tramp_ioctl(int __fd, unsigned long int __request, void * arg));

in function:
TRAMPOLINE_HOOK(ioctl)
*/

#define HIJACK_SIZE 12


#define TRAMPOLINE_FUNCTION(target, trampoline) \
	void tramp_b_##target();			\
	trampoline;							\
	asm ("tramp_b_"#target":");			\
	asm ("ldr pc, [pc, #0]");			\
	asm ("mov r0, r0");					\
	asm ("mov r0, r0");					\
	asm ("mov r0, r0");					\
	asm ("tramp_"#target":");			\
	asm ("mov r1, r1");  				\
	asm ("mov r1, r1");					\
	asm ("mov r1, r1");					\
	asm ("ldr pc,[pc,#0]");				\
	asm ("mov r1, r1");  				\
	asm ("mov r1, r1");					\
	asm ("mov r0, r0");


int armv7_inline_hook(void *target, void *tramp, void *fake);

#define TRAMPOLINE_HOOK(target) armv7_inline_hook(target, tramp_b_##target, fake_##target)
