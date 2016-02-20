#include <math.h>
#include "drawing.h"

struct complex_pixel {
	char *r;
	char *g;
	char *b;
	int  *x;
	int  *y; 

};

struct Complex {
	double real;
	double imaginary;
};

ImageBuffer buff_bro = {0};

Complex complex_square(Complex c);

void setup()
{
	Width = 600;
	Height = 600;
	buff_bro = new_image_buffer(Width, Height);
}


Complex complex_square(Complex c)
{
	Complex Result = {0};

	Result.real = ((c.real*c.real)-(c.imaginary*c.imaginary));
	Result.imaginary = (2*c.real*c.imaginary);

	return Result;
}

static int st_i;

Color mandelbrot(int x, int y)
{
	Color color = {0};

	Complex c = {((double)(x-Width/2))/100, ((double)(y-Height/2))/100};

	Complex prev_ans = c;

	for(int i = 0; i < 20; ++i)
	{
		Complex prev_ans_squared = complex_square(prev_ans);

		prev_ans.real = prev_ans_squared.real + c.real;
		prev_ans.imaginary = prev_ans_squared.imaginary + c.imaginary;

	}

	double radius = sqrt(prev_ans.real*prev_ans.real + prev_ans.imaginary*prev_ans.imaginary);

	if(radius > 2) color.b = 255; 

	return color;
}

void draw()
{
	unsigned int* pixel = (unsigned int*)buff_bro.Memory;
	
	++st_i;

	for(int y = 0; y < Height; ++y)
	{
		for(int x = 0; x < Width; ++x)
		{
			Color color = mandelbrot(x,y);

			*pixel++ = color.b | color.g << 8 | color.r << 16;
		}
	}
	
	render_image(&buff_bro);
}

// void end(){}