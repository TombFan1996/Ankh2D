#include "tmx_parser.h"

tmx_map* tmx_parser_create(const char* _filename)
{
	tmx_map* _tmxm = (tmx_map*)malloc(sizeof(tmx_map));

	FILE* file;
	file = fopen(_filename, "rb");

	fread(&_tmxm->signiture, 4, 1, file);

	if (_tmxm->signiture[0] == 'T' && _tmxm->signiture[1] == 'M' &&
		_tmxm->signiture[2] == 'X' && _tmxm->signiture[3] == '.')
	{
		fread(&_tmxm->map_width, 2, 1, file);
		fread(&_tmxm->map_height, 2, 1, file);
	
		fread(&_tmxm->tile_width, 1, 1, file);
		fread(&_tmxm->tile_height, 1, 1, file);

		fread(&_tmxm->num_tilesets, 1, 1, file);
		fread(&_tmxm->num_layers, 1, 1, file);

		_tmxm->tileset = (TILESET*)malloc(sizeof(TILESET) * _tmxm->num_tilesets);

		for (uint8_t i = 0; i < _tmxm->num_tilesets; i++)
		{
			fread(&_tmxm->tileset[i].firstGid, 1, 1, file);
			fread(&_tmxm->tileset[i].name, 10, 1, file);
			fread(&_tmxm->tileset[i].tile_width, 2, 1, file);
			fread(&_tmxm->tileset[i].tile_height, 2, 1, file);
			fread(&_tmxm->tileset[i].filename, 25, 1, file);
		}

		_tmxm->layer = (LAYER*)malloc(sizeof(LAYER) * _tmxm->num_layers);

		for (uint8_t i = 0; i < _tmxm->num_layers; i++)
		{
			fread(&_tmxm->layer[i].name, 10, 1, file);
			fread(&_tmxm->layer[i].width, 2, 1, file);
			fread(&_tmxm->layer[i].height, 2, 1, file);

			uint32_t data_size = sizeof(uint16_t) * (_tmxm->layer[i].width * _tmxm->layer[i].height);
			_tmxm->layer[i].data = (uint16_t*)malloc(data_size);
			fread(&_tmxm->layer[i].data[0], data_size, 1, file);
		}

		fclose(file);
		return _tmxm;
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