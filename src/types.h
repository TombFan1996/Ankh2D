#ifndef TYPES_H
#define TYPES_H

#if ANKH2D_PSX
	#include <stdlib.h>
	#include <libgte.h>
	#include <libgpu.h>
	#include <libgs.h>

	#define false 0
	#define true 1

	//8 bit types
	typedef char int8_t;
	typedef unsigned char uint8_t;

	//16 bit types
	typedef short int16_t;
	typedef unsigned short uint16_t;
	
	//32 bit types
	typedef int int32_t;
	typedef unsigned int uint32_t;

	//64 bit types
	typedef long long int int64_t;
	typedef unsigned long long int uint64_t;
#endif

#endif