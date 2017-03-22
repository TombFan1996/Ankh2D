#ifndef TYPES_H
#define TYPES_H

#if ANKH2D_PSX
	#include <stdlib.h>

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

	typedef struct 
	{
		int8_t x;
		int8_t y;
	}int8_vec2;

	typedef struct 
	{
		int8_t x;
		int8_t y;
		int8_t z;
	}int8_vec3;

	typedef struct 
	{
		int16_t x;
		int16_t y;
	}int16_vec2;

	typedef struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
	}int16_vec3;
#endif

#endif