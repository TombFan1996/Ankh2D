#include "tmx_parser.h"

#if ANKH2D_WIN32
	tmx_map* tmx_parser_create(const char* _filename)
	{
		tmx_map* tmxm = (tmx_map*)malloc(sizeof(tmx_map));

		FILE* file;
		file = fopen(_filename, "rb");

		if (file == NULL)
			log_fprint("ERROR: %s doesn't exist", _filename);

		fread(&tmxm->signiture, 4, 1, file);

		if (tmxm->signiture[0] == 'T' && tmxm->signiture[1] == 'M' &&
			tmxm->signiture[2] == 'X' && tmxm->signiture[3] == '.')
		{
			fread(&tmxm->map_width, 2, 1, file);
			fread(&tmxm->map_height, 2, 1, file);
	
			fread(&tmxm->tile_width, 1, 1, file);
			fread(&tmxm->tile_height, 1, 1, file);

			fread(&tmxm->num_collisions, 1, 1, file);
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

			if (tmxm->num_collisions != 0)
			{
				uint32_t data_size = sizeof(uint8_t) * (tmxm->layer[0].width * tmxm->layer[0].height);
				tmxm->collision_data = (uint8_t*)malloc(data_size);
				fread(&tmxm->collision_data[0], data_size, 1, file);

				//setup the collisions for sprite/collision checks later
				tmx_parser_setup_collisions(tmxm);
			}
		
			log_fprint("'%s' successfully created", _filename);

			fclose(file);
			return tmxm;
		}

		else
		{
			fclose(file);
			return nullptr;
		}
	}

	void tmx_parser_setup_collisions(tmx_map* _tmxm)
	{
		uint8_t current_x = 0, current_y = 0;
		_tmxm->collision_coords = (vec2*)malloc(sizeof(vec2) * _tmxm->num_collisions);
		uint16_t current_collision_index = 0;
		for (uint16_t i = 0; i < (_tmxm->map_height * _tmxm->map_width); i++)
		{
			//if this tile has a collision on it
			if (_tmxm->collision_data[i] == 1)
			{
				_tmxm->collision_coords[current_collision_index].x = current_x;
				_tmxm->collision_coords[current_collision_index].y = current_y;
				current_collision_index++;
			}

			current_x++;
			if (current_x == _tmxm->map_width)
			{
				current_x = 0;
				current_y++;
			}
		}
	}

	void tmx_parser_destroy(tmx_map* _tmxm)
	{
		delete _tmxm;
		_tmxm = NULL;
	}

#elif ANKH2D_PSX

#endif