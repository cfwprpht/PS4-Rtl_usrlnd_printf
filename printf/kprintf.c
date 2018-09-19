/*
*
*        _       _________ ______            _______  _______  _______  ______   _______  _______
*       ( \      \__   __/(  ___ \ |\     /|(  ___  )(       )(  ____ \(  ___ \ (  ____ )(  ____ \|\     /|
*       | (         ) (   | (   ) )| )   ( || (   ) || () () || (    \/| (   ) )| (    )|| (    \/| )   ( |
*       | |         | |   | (__/ / | (___) || |   | || || || || (__    | (__/ / | (____)|| (__    | | _ | |
*       | |         | |   |  __ (  |  ___  || |   | || |(_)| ||  __)   |  __ (  |     __)|  __)   | |( )| |
*       | |         | |   | (  \ \ | (   ) || |   | || |   | || (      | (  \ \ | (\ (   | (      | || || |
*       | (____/\___) (___| )___) )| )   ( || (___) || )   ( || (____/\| )___) )| ) \ \__| (____/\| () () |
*       (_______/\_______/|/ \___/ |/     \|(_______)|/     \|(_______/|/ \___/ |/   \__/(_______/(_______)
*
*
*
*/

#include "kprintf.h"

#define FW_505

#if defined FW_455
#define __Xfast_syscall			   0x3095D0
#define __printf                   0x17F30
#define __copyin                   0x14A890
#elif defined FW_500 || FW_501
#define __Xfast_syscall			   0x1C0
#define __printf                   0x435C70
#define __copyin                   0x1EA600
#elif defined FW_505
#define __Xfast_syscall			   0x1C0
#define __printf                   0x436040
#define __copyin                   0x1EA710
#endif

// Userland Buffer for this program.
static char buffer[1024];

// Get Kernel Base Offset.
unsigned int long long __readmsr(unsigned long __register) {
	// Loads the contents of a 64-bit model specific register (MSR) specified in
	// the ECX register into registers EDX:EAX. The EDX register is loaded with
	// the high-order 32 bits of the MSR and the EAX register is loaded with the
	// low-order 32 bits. If less than 64 bits are implemented in the MSR being
	// read, the values returned to EDX:EAX in unimplemented bit locations are
	// undefined.
	unsigned long __edx;
	unsigned long __eax;
	__asm__("rdmsr" : "=d"(__edx), "=a"(__eax) : "c"(__register));
	return (((unsigned int long long)__edx) << 32) | (unsigned int long long)__eax;
}

// The kernel payload for this program code.
void *kprintf(struct thread *td) {
	// Get Base Offset.
	void *kern_base = &((uint8_t *)__readmsr(0xC0000082))[-__Xfast_syscall];

	// Resolve Macro.
    #define r(name, offset) name = (void *)(kern_base + offset);

	// Define Symbols.
	int (*copyin)(const void *uaddr, void *kaddr, size_t len) = 0;
	int (*printf)(const char *format, ...) = 0;

	// Resolve Functions.
	r(copyin, __copyin);
	r(printf, __printf);

	// Copy data into kernel space.
	char kernel_buffer[1024];
	copyin(&buffer, &kernel_buffer, 1024);

	// Call printf.
	printf(kernel_buffer);
	return 0;
}

// A internal mem copy since including <string> fucks up the whole source.
static inline void *memcpy(void *dst, const void *src, size_t len) {
	size_t i;
	for (i = 0; i < len; i++)
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
	return dst;
}

// Copy message for printf into "_printf" program space.
void kpayload_copyin(char *buff, int len) {
	memcpy(buffer, buff, len);
	buffer[len] = '\0';
}