#include "bmp.h"

BMP_24* bmp_24_load(const char* _filename)
{
	FILE* file = fopen(_filename, "rb");
	
	if (file != NULL)
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

	else
	{
		fclose(file);
		log_fprint("ERROR: %s does not exist", _filename);
	}

	return NULL;
}

RGB_DATA* bmp_24_bgr_rgb(BMP_24* _bmp)
{
	uint32_t size = sizeof(RGB_DATA) * (_bmp->ih.height * _bmp->ih.width);
	RGB_DATA* rgb_data = (RGB_DATA*)malloc(sizeof(RGB_DATA) * size);
	for (uint32_t i = 0; i < (_bmp->ih.height * _bmp->ih.width); i++){
		memcpy(&rgb_data[i], &_bmp->pd[i], 4);
	}

	return rgb_data;
}