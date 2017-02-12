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
	uint8_t num_collisions;
	uint8_t num_tilesets;
	uint8_t num_layers;
	std::vector<TILESET> tileset;
	std::vector<LAYER> layer;
	uint8_t* collision_data;
} tmx_map;

tmx_map* tmx_parse_xml(const char* _filename)
{
	tmx_map* new_map = new tmx_map;
	tinyxml2::XMLDocument m_xmlMap;
	m_xmlMap.LoadFile(_filename);

	//track how many collisions are present in the map
	uint8_t col_counter = 0;

	//get the generic map data
	tinyxml2::XMLNode* map = m_xmlMap.FirstChildElement("map");
	tinyxml2::XMLElement* mapElem = map->ToElement();

	new_map->map_height = mapElem->IntAttribute("height");
	new_map->map_width = mapElem->IntAttribute("width");
	new_map->tile_width = mapElem->IntAttribute("tilewidth");
	new_map->tile_height = mapElem->IntAttribute("tileheight");

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

		new_map->tileset.push_back(tileset);
	}

	//get all the layers in the map
	for (tinyxml2::XMLElement* childLayer = map->FirstChildElement("layer"); childLayer != NULL; 
		childLayer = childLayer->NextSiblingElement("layer"))
	{
		LAYER layer;
		tinyxml2::XMLElement* layerElement = childLayer->ToElement();

		char* name = const_cast<char*>(layerElement->Attribute("name"));
		
		if (strcmp(name, "collisions") == 0)
		{
			uint16_t width = layerElement->IntAttribute("width");
			uint16_t height = layerElement->IntAttribute("height");

			//create the new set of data
			new_map->collision_data = new uint8_t[width * height];

			uint16_t col_iter = 0;
			//get all the data from this layer
			tinyxml2::XMLNode* data = childLayer->FirstChildElement("data");
			for (tinyxml2::XMLElement* child = data->FirstChildElement("tile"); child != NULL; 
				child = child->NextSiblingElement("tile"))
			{
				uint8_t new_tile;
				uint16_t temp_tile = child->IntAttribute("gid");
				//make it binary 0 or 1 to pack it in to 1 byte datatype
				if (temp_tile != 0)
				{
					col_counter++;
					new_tile = 1;
				}

				else
					new_tile = 0;

				new_map->collision_data[col_iter] = new_tile;
				col_iter++;
			}
		}

		//if this isnt a collision layer and a normal layer
		//continue parsing the data
		else
		{
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

			new_map->layer.push_back(layer);
		}
	}

	char* sig = "TMX.";
	memcpy(&new_map->signiture, sig, 4);
	new_map->num_collisions = col_counter;
	new_map->num_tilesets = new_map->tileset.size();
	new_map->num_layers = new_map->layer.size();
	return new_map;
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

	fwrite(&_tmxm->num_collisions, 1, 1, file);
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

	//used the layers width and height to calculate all the tiles
	//optimisation would be to no process all of the tiles!
	if (_tmxm->num_collisions != 0){
		uint32_t data_size = sizeof(uint8_t) * (_tmxm->layer[0].width * _tmxm->layer[0].height);
		fwrite(&_tmxm->collision_data[0], data_size, 1, file);
	}

	fclose(file);
}

int main(int argc, char** argv)
{
	//no params, just the executable name
	if (argc == 1)
		return 0;

	tmx_map* map = tmx_parse_xml(argv[1]);
	//tmx_map* map = tmx_parse_xml("test_2.tmx");
	
	if (map != NULL)
	{
		tmx_write_binary(argv[1], map);
		//tmx_write_binary("test_2.tmx", map);
		delete map;
		map = nullptr;
	}

	return 0;
}