#include "graphics.h"
#include "sprite.h"

//raw image header file
#include "image.h"

//holds the header file from the TIM file
//converted via bin2h
extern uint8_t image[];

//Note: this is for strictly testing ANKH2D_PSX!

int main(int argc, char** argv)
{
	//both the graphics and the camera can be used throughout
	graphics main_graphics;
	graphics_create(&main_graphics, 0);

	sprite new_sprite;
	transform sprite_trans;
	transform_create(&sprite_trans, int16_vec2_create(10, 10);
	sprite_create(&new_sprite, sprite_trans, (uint32_t*)(image+4), (uint32_t*)0);

	while (graphics_check_quit(&main_graphics))
	{
		graphics_pre_draw(&main_graphics);
		FntPrint("Ankh2D Demo Text");
		sprite_draw(&main_graphics.ot[main_graphics.current_buffer], &new_sprite);
		graphics_update(&main_graphics);
	}

	graphics_destroy(&main_graphics);

	return 0;
}