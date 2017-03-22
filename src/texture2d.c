#include "texture2d.h"

#if ANKH2D_WIN32
	texture2d texture2d_create(const char* _filename)
	{
		texture2d new_tex;

		BMP_24* texture = bmp_24_load(_filename);
		RGBA_DATA* texture_rgba = bmp_24_bgr_rgba(texture);

		new_tex.width = texture->ih.width;
		new_tex.height = texture->ih.height;

		//create and bind texture
		glGenTextures(1, &new_tex.texture);
		glBindTexture(GL_TEXTURE_2D, new_tex.texture);

		//if we read outside the texture size, it will start again (wrap repeat)
		//use GL_CLAMP for a default colour to be used outside bounds of tex
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//if texture is bigger than specified or rotate, linearly interpolate
		//GL_NEAREST give no texture filtering.
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//construct the texture image
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, new_tex.width, new_tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_rgba); 

		free(texture_rgba);
		free(texture);

		log_fprint("'%s' successfully created", _filename);

		return new_tex;
	}

	void texture2d_bind(texture2d* _tex)
	{
		//change which texture GL is working w/
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _tex->texture);
	}

	void texture2d_destroy(texture2d* _tex)
	{
		glDeleteTextures(1, &_tex->texture);
	}

#elif ANKH2D_PSX

	void texture2d_create(texture2d* _tex, uint32_t* _tpage, uint32_t* _clut)
	{
		//_tpage = texture page location
		GsGetTimInfo((uint32_t*)(_tpage+4), &_tex->tim);

		//Load pattern into VRAM
		_tex->rect.x = _tex->tim.px;
   		_tex->rect.y = _tex->tim.py;
   		_tex->rect.w = _tex->tim.pw;
   		_tex->rect.h = _tex->tim.ph;
   		LoadImage(&_tex->rect, _tex->tim.pixel); 

   		//Load CLUT into VRAM
   		_tex->rect.x = _tex->tim.cx;
   		_tex->rect.y = _tex->tim.cy;
   		_tex->rect.w = _tex->tim.cw;
   		_tex->rect.h = _tex->tim.ch;
   		LoadImage(&_tex->rect, _tex->tim.clut); 

   		//Return updated TPage and Clut IDs
   		(*_tpage) = GetTPage(_tex->tim.pmode, 1, _tex->tim.px, _tex->tim.py);
   		(*_clut) = GetClut(_tex->tim.cx, _tex->tim.cy);
	}
	
#endif