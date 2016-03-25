#ifndef _drawing_
#define _drawing_

void setup();
void draw();
#include "image_win32.cpp"
#include <math.h>


struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;

	Color& operator+=(const Color& c)
	{
		this->r += (this->r + c.r) / 2;
		this->g += (this->g + c.g) / 2;
		this->b += (this->b + c.b) / 2;

		return *this;		
	}
};

int rnd(int n) {return rand()%n;}

void render_image(ImageBuffer* Image, int x = 0, int y = 0)
{
	ImageBuffer* Background = &stage;

	if(y+Image->Height < 0 
	|| x+Image->Width < 0 
	|| y > Background->Height 
	|| x > Background->Width)
		return;


	unsigned char* imgRow = (unsigned char*)Image->Memory;
	unsigned char* bgRow = (unsigned char*)Background->Memory;
	

	if(y > 0)
		bgRow += Background->Pitch*y;

	for(int imgY = 0; imgY < Image->Height && y + imgY < Background->Height; ++imgY)
	{
		if(y + imgY >= 0)
		{
			unsigned int* bgPx = (unsigned int*)bgRow;
			unsigned int* imgPx = (unsigned int*) imgRow;

			for(int iX = 0; iX < Background->Width; iX++)
			{
				if(iX >= x && iX < x + Image->Width)
					*bgPx = *imgPx++; 
				
				bgPx++;
			}


			bgRow += Background->Pitch;
		}
		
		imgRow += Image->Pitch;
	}
}

void rect(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b){
	ImageBuffer img = new_image_buffer(w, h);

	unsigned int* pixel = (unsigned int*)img.Memory;
	for(int n = 0; n < w*h; n++)
	{
		*pixel++ = b | g << 8 | r << 16;
	}

	render_image(&img, x, y);

	delete_image_buffer(&img);
}

// void rect(int x, int y, int w, int h, Color c)
// {
// 	rect(x, y, w, h, c.r, c.g, c.b);	
// }

// void circle(int x, int y, int r, unsigned char r, unsigned char g, unsigned char b){
	
// }

// void circle(int x, int y, int r, Color c){
// 	 circle(x, y, r, c.r, c.g, c.b);
// }


#endif