#include "tmx_parser.h"

tmx_map* tmx_parser_create(const char* _filename)
{
	tmx_map* tmxm = (tmx_map*)malloc(sizeof(tmx_map));

	FILE* file;
	file = fopen(_filename, "rb");

	fread(&tmxm->signiture, 4, 1, file);

	if (tmxm->signiture[0] == 'T' && tmxm->signiture[1] == 'M' &&
		tmxm->signiture[2] == 'X' && tmxm->signiture[3] == '.')
	{
		fread(&tmxm->map_width, 2, 1, file);
		fread(&tmxm->map_height, 2, 1, file);
	
		fread(&tmxm->tile_width, 1, 1, file);
		fread(&tmxm->tile_height, 1, 1, file);

		fread(&tmxm->num_tilesets, 1, 1, file);
		fread(&tmxm->num_layers, 1, 1, file);

		tmxm->tileset = (TILESET*)malloc(sizeof(TILESET) * tmxm->num_tilesets);

		for (uint8_t i = 0; i < tmxm->num_tilesets; i++)
		{
			fread(&tmxm->tileset[i].firstGid, 1, 1, file);
			fread(&tmxm->tileset[i].name, 10, 1, file);
			fread(&tmxm->tileset[i].tile_width, 2, 1, file);
			fread(&tmxm->tileset[i].tile_height, 2, 1, file);
			fread(&tmxm->tileset[i].filename, 25, 1, file);
		}

		tmxm->layer = (LAYER*)malloc(sizeof(LAYER) * tmxm->num_layers);

		for (uint8_t i = 0; i < tmxm->num_layers; i++)
		{
			fread(&tmxm->layer[i].name, 10, 1, file);
			fread(&tmxm->layer[i].width, 2, 1, file);
			fread(&tmxm->layer[i].height, 2, 1, file);

			uint32_t data_size = sizeof(uint16_t) * (tmxm->layer[i].width * tmxm->layer[i].height);
			tmxm->layer[i].data = (uint16_t*)malloc(data_size);
			fread(&tmxm->layer[i].data[0], data_size, 1, file);
		}

		fclose(file);
		return tmxm;
	}

	else
	{
		fclose(file);
		return nullptr;
	}
}

void tmx_parser_destroy(tmx_map* _tmxm)
{
	delete _tmxm;
	_tmxm = NULL;
}