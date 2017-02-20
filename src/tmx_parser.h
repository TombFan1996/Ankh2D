#ifndef TMX_PARSER_H
#define TMX_PARSER_H

#if ANKH2D_WIN32
	#include <stdint.h>
	#include <malloc.h>
	#include "logger.h"
	#include "bmath.h"

	//this is a lite version of the tmx file format
	//uses tinyxml2 and tiled map editor (0.18)
	//http://doc.mapeditor.org/reference/tmx-map-format/ 

	//improvements
	//only loads XML formatted tmx files (look into CSV, base64)
	//remove the tilesize in the individual tilesets (should be the same as the map tilesize)
	//add rotational code to the tmx parser for the TILE data
	//adjust the buffer size of the names and filenames
	//concatenate the image filename dir path (some are wild)

	typedef struct
	{
		uint8_t firstGid;
		char name[10];
		uint16_t tile_width;
		uint16_t tile_height;
		char filename[25];
	} TILESET;

	typedef struct
	{
		char name[10];
		uint16_t width;
		uint16_t height;
		uint16_t* data;
	} LAYER;

	typedef struct
	{
		char signiture[4];
		uint16_t map_width;
		uint16_t map_height;
		uint8_t tile_width;
		uint8_t tile_height;
		uint8_t num_collisions;
		uint8_t num_tilesets;
		uint8_t num_layers;
		TILESET* tileset;
		LAYER* layer;
		uint8_t* collision_data;
		vec2* collision_coords;
	} tmx_map;

	tmx_map* tmx_parser_create(const char* _filename);
	//pay the price for collision upfront, this way we don't 
	//need to loop through every tile each frame (killed my pc on 20 x 20 map)
	void tmx_parser_setup_collisions(tmx_map* _tmxm);
	void tmx_parser_destroy(tmx_map* _tmxm);

#elif ANKH2D_PSX

#endif

#endif