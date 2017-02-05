#include "tmx_parser.h"

tmx_map* tmx_parser_create(const char* _filename)
{
	tmx_map* newMap = (tmx_map*)malloc(sizeof(tmx_map));
	tinyxml2::XMLDocument m_xmlMap;
	m_xmlMap.LoadFile(_filename);

	if (m_xmlMap.Error()){
		//tinyxml2 has shuddy error logging
		log_fprint("%s", m_xmlMap.GetErrorStr1());
	}

	else
	{
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
			tinyxml2::XMLElement* tilesetElement = childTileset->ToElement();

			newMap->tileset.firstGid = tilesetElement->IntAttribute("firstgid");
			
			char* name = const_cast<char*>(tilesetElement->Attribute("name"));
			memcpy(newMap->tileset.name, name, sizeof(newMap->tileset.name));
			
			newMap->tileset.tile_width = tilesetElement->IntAttribute("tilewidth");
			newMap->tileset.tile_height = tilesetElement->IntAttribute("tileheight");

			char* filename = const_cast<char*>(tilesetElement->FirstChildElement("image")->Attribute("source"));
			memcpy(newMap->tileset.filename, filename, sizeof(newMap->tileset.filename));
		}

		//get all the layers in the map
		for (tinyxml2::XMLElement* childLayer = map->FirstChildElement("layer"); childLayer != NULL; 
			childLayer = childLayer->NextSiblingElement("layer"))
		{
			tinyxml2::XMLElement* layerElement = childLayer->ToElement();

			char* name = const_cast<char*>(layerElement->Attribute("name"));
			memcpy(newMap->layer.name, name, sizeof(newMap->layer.name));

			newMap->layer.width = layerElement->IntAttribute("width");
			newMap->layer.height = layerElement->IntAttribute("height");

			//create the new set of data
			newMap->layer.data = (TILE*)malloc(newMap->layer.width * newMap->layer.height);
			uint16_t tile_iter = 0;

			//get all the data from this layer
			tinyxml2::XMLNode* data = childLayer->FirstChildElement("data");
			for (tinyxml2::XMLElement* child = data->FirstChildElement("tile"); child != NULL; 
				child = child->NextSiblingElement("tile"))
			{
				TILE newTile;
				newTile.tile_id = child->IntAttribute("gid");
				newMap->layer.data[tile_iter] = newTile;
				tile_iter++;

				//TODO.. Add all of the rotation tile code here...
			}

			//check if the data in the layer is in XML format
			if (tile_iter == 0)
				log_fprint("Export the TMX file as XML layer format");
		}
	}

	return newMap;
}

void tmx_parser_destroy(tmx_map* _tmxm)
{
	free(_tmxm);
	_tmxm = NULL;
}