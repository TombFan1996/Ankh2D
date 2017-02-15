#ifndef TMX_PARSER_H
#define TMX_PARSER_H

#include <stdint.h>
#include <malloc.h>
#include "logger.h"

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
} tmx_map;

tmx_map* tmx_parser_create(const char* _filename);
void tmx_parser_destroy(tmx_map* _tmxm);

#endif