#include "bmp.h"

BMP_24* bmp_24_load(const char* _filename)
{
	FILE* file = fopen(_filename, "rb");
	
	if (file == NULL)
		log_fprint("ERROR: %s doesn't exist", _filename);

	else
	{
		//read the signiture to see if this is a BMP file
		//then move file pointer back to start
		char bmp_sig[2];
		fread(&bmp_sig, 2, 1, file);
		rewind(file);

		if (bmp_sig[0] == 'B' && bmp_sig[1] == 'M')
		{
			BMP_24* new_bmp_24 = (BMP_24*)malloc(sizeof(BMP_24));

			//read the file header in
			fread(&new_bmp_24->fh, sizeof(BMP_FILE_HEADER), 1, file);

			//read the image header in
			fread(&new_bmp_24->ih, sizeof(BMP_IMAGE_HEADER), 1, file);

			if (new_bmp_24->ih.bit_count == 24)
			{
				//move file pointer to the start of pixel data
				fseek(file, new_bmp_24->fh.pixel_offset, SEEK_SET);

				//read the pixel data in
				uint32_t ct_size = new_bmp_24->ih.image_size;
				new_bmp_24->pd = (BMP_PIXEL_24_DATA*)malloc(ct_size);
				fread(&new_bmp_24->pd[0], ct_size, 1, file);

				return new_bmp_24;
			}

			else
			{
				log_fprint("ERROR: %s bit depth is %i, not 24", _filename, new_bmp_24->ih.bit_count);
				free(new_bmp_24);
				fclose(file);
			}
		}

		else
		{
			fclose(file);
			log_fprint("ERROR: %s is not a valid BMP file", _filename);
		}
	}

	fclose(file);
	return NULL;
}

RGBA_DATA* bmp_24_bgr_rgba(BMP_24* _bmp)
{
	uint32_t size = sizeof(RGBA_DATA) * (_bmp->ih.height * _bmp->ih.width);
	RGBA_DATA* rgba_data = (RGBA_DATA*)malloc(sizeof(RGBA_DATA) * size);
	for (uint32_t i = 0; i < (_bmp->ih.height * _bmp->ih.width); i++)
	{
		rgba_data[i].red = _bmp->pd[i].red;
		rgba_data[i].green = _bmp->pd[i].green;
		rgba_data[i].blue = _bmp->pd[i].blue;

		//use black as the alpha colour for now
		//get this data from elsewhere
		if (rgba_data[i].red == alpha_colour_key.x && rgba_data[i].green == alpha_colour_key.y && rgba_data[i].blue == alpha_colour_key.z)
			rgba_data[i].alpha = 0;
		else
			rgba_data[i].alpha = 255;
	}

	return rgba_data;
}

void bmp_set_alpha_color(vec3 _colour)
{
	alpha_colour_key = _colour;
}