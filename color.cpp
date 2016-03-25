#include "drawing.h"

ImageBuffer buff = {};

Color c1, c2 = {};

void setup(){
	// draw_once = true;
	c2.r = 255;
	c1.b = 255;
	buff = new_image_buffer(Width, Height);
}



void draw(){

	unsigned int* pixel = (unsigned int*)buff.Memory;
		
	


	c1 += c2;
	Sleep(1000);

	for(int y = 0; y < buff.Height; ++y)
	{
		for(int x = 0; x < buff.Width; ++x)
		{
				*pixel++ = c1.b | c1.g << 8 | c1.r << 16;
		}
	}


	render_image(&buff);




}
