#ifndef TMX_PARSER_H
#define TMX_PARSER_H

#include <stdint.h>
#include <vector>
#include "tinyxml2.h"
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
	char name[128];
	uint8_t tile_width;
	uint8_t tile_height;
	char filename[128];
} TILESET;

typedef struct
{
	uint16_t tile_id;
} TILE;

typedef struct
{
	char name[128];
	uint16_t width;
	uint16_t height;
	std::vector<TILE> data;
} LAYER;

typedef struct
{
	uint16_t map_width;
	uint16_t map_height;
	uint8_t tile_width;
	uint8_t tile_height;
	std::vector<TILESET> tileset;
	std::vector<LAYER> layer;
} TMX_MAP;

class TMX_Parser
{
	public:
		TMX_Parser(const char* _name);
		TMX_MAP getMap(){return m_map;}
		~TMX_Parser();
	private:
		TMX_MAP m_map;
};

#endif