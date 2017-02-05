#include <stdint.h>
#include <vector>
#include "tinyxml2.h"

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
	uint8_t num_tilesets;
	uint8_t num_layers;
	std::vector<TILESET> tileset;
	std::vector<LAYER> layer;
} tmx_map;

tmx_map* tmx_parse_xml(const char* _filename)
{
	tmx_map* newMap = new tmx_map;
	tinyxml2::XMLDocument m_xmlMap;
	m_xmlMap.LoadFile(_filename);

	//get the generic map data
	tinyxml2::XMLNode* map = m_xmlMap.FirstChildElement("map");
	tinyxml2::XMLElement* mapElem = map->ToElement();

	newMap->map_height = mapElem->IntAttribute("height");
	newMap->map_width = mapElem->IntAttribute("width");
	newMap->tile_width = mapElem->IntAttribute("tilewidth");
	newMap->tile_height = mapElem->IntAttribute("tileheight");

	//get the tilesets used in the map
	for (tinyxml2::XMLElement* childTileset = map->FirstChildElement("tileset"); childTileset != NULL; 
		childTileset = childTileset->NextSiblingElement("tileset"))
	{
		TILESET tileset;
		tinyxml2::XMLElement* tilesetElement = childTileset->ToElement();

		tileset.firstGid = tilesetElement->IntAttribute("firstgid");
			
		char* name = const_cast<char*>(tilesetElement->Attribute("name"));
		memcpy(tileset.name, name, sizeof(tileset.name));
			
		tileset.tile_width = tilesetElement->IntAttribute("tilewidth");
		tileset.tile_height = tilesetElement->IntAttribute("tileheight");

		char* filename = const_cast<char*>(tilesetElement->FirstChildElement("image")->Attribute("source"));
		memcpy(tileset.filename, filename, sizeof(tileset.filename));

		newMap->tileset.push_back(tileset);
	}

	//get all the layers in the map
	for (tinyxml2::XMLElement* childLayer = map->FirstChildElement("layer"); childLayer != NULL; 
		childLayer = childLayer->NextSiblingElement("layer"))
	{
		LAYER layer;
		tinyxml2::XMLElement* layerElement = childLayer->ToElement();

		char* name = const_cast<char*>(layerElement->Attribute("name"));
		memcpy(layer.name, name, 10);

		layer.width = layerElement->IntAttribute("width");
		layer.height = layerElement->IntAttribute("height");

		//create the new set of data
		layer.data = new uint16_t[layer.width * layer.height];
		uint16_t tile_iter = 0;

		//get all the data from this layer
		tinyxml2::XMLNode* data = childLayer->FirstChildElement("data");
		for (tinyxml2::XMLElement* child = data->FirstChildElement("tile"); child != NULL; 
			child = child->NextSiblingElement("tile"))
		{
			uint16_t newTile;
			newTile = child->IntAttribute("gid");
			layer.data[tile_iter] = newTile;
			tile_iter++;

			//TODO.. Add all of the rotation tile code here...
		}

		//this means that the XML format isn't in use
		//therefore we return nothing and quit the conversion
		if (tile_iter == 0)
			return NULL;

		newMap->layer.push_back(layer);
	}

	char* sig = "TMX.";
	strcpy(newMap->signiture, sig);
	newMap->num_tilesets = newMap->tileset.size();
	newMap->num_layers = newMap->layer.size();
	return newMap;
}

void tmx_write_binary(const char* _filename, tmx_map* _tmxm)
{
	uint8_t size = std::strlen(_filename);
	char* new_name = (char*)malloc(sizeof(size));
	std::strcpy(new_name, _filename);
	new_name[size-3] = 'b';
	new_name[size-2] = 'i';
	new_name[size-1] = 'n';

	FILE* file;
	file = fopen(new_name, "wb");

	fwrite(_tmxm->signiture, 4, 1, file);

	fwrite(&_tmxm->map_width, 2, 1, file);
	fwrite(&_tmxm->map_height, 2, 1, file);
	
	fwrite(&_tmxm->tile_width, 1, 1, file);
	fwrite(&_tmxm->tile_height, 1, 1, file);

	fwrite(&_tmxm->num_tilesets, 1, 1, file);
	fwrite(&_tmxm->num_layers, 1, 1, file);

	for (uint8_t i = 0; i < _tmxm->num_tilesets; i++)
	{
		fwrite(&_tmxm->tileset[i].firstGid, 1, 1, file);
		fwrite(&_tmxm->tileset[i].name, 10, 1, file);
		fwrite(&_tmxm->tileset[i].tile_width, 2, 1, file);
		fwrite(&_tmxm->tileset[i].tile_height, 2, 1, file);
		fwrite(&_tmxm->tileset[i].filename, 25, 1, file);
	}

	for (uint8_t i = 0; i < _tmxm->num_layers; i++)
	{
		fwrite(&_tmxm->layer[i].name, 10, 1, file);
		fwrite(&_tmxm->layer[i].width, 2, 1, file);
		fwrite(&_tmxm->layer[i].height, 2, 1, file);
		uint32_t data_size = sizeof(uint16_t) * (_tmxm->layer[i].width * _tmxm->layer[i].height);
		fwrite(&_tmxm->layer[i].data[0], data_size, 1, file);
	}

	fclose(file);
}

int main(int argc, char** argv)
{
	//no params, just the executable name
	if (argc == 1)
		return 0;

	tmx_map* map = tmx_parse_xml(argv[1]);
	
	if (map != NULL)
	{
		tmx_write_binary(argv[1], map);
		delete map;
		map = nullptr;
	}

	return 0;
}