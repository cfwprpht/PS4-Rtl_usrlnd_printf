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

#include <cstdio>
#include <string.h>
#include "syscall.h"

#include "kprintf.h"

#define LIBRARY_IMPL (1)
#include "printf.h"

// Our User Land printf fix.
PRX_INTERFACE void _printf(const char *format, ...) {
	// Setup a Kernel Buffer.
	char buff[1024];

	// Setup a variadict list for the arguments.
	va_list args;

	// Load Format and arguments.
	va_start(args, format);

	// Copy all together.
	vsnprintf(buff, sizeof(buff), format, args);

	// Write into kpayload buffer.
	kpayload_copyin(buff, strlen(buff));

	// Call syscall 11.
	struct thread td;
	syscall(11, (void *)&kprintf, &td);

	// Free variadict list.
	va_end(args);
}