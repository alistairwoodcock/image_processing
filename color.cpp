#include "drawing.h"

ImageBuffer buff = {};

Color c1, c2 = {};

void setup(){
	// draw_once = true;
	c2.r = 255;
	c1.b = 255;
	buff = new_image_buffer(Width, Height);
}

static int i; 
static int j;

void draw(){
	clear_image(&buff);
	//line(300,20,i,200, c1);
	// line(300,300,300,600-i, c1);
	// set_pixel(&buff, 100, 100, c1);
	empty_triangle(&buff, 100+i,100,50,150-i,150-i,150,255,0,0);
	// // render_image(&buff, j);

	i+=2;
	write_log("%d", i);

	// if(i >= Height) 
	// {
	// 	++j;
	// 	i = 0;
	// }

	Sleep(100);
	render_image(&buff);
}
