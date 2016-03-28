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

void clear_image(ImageBuffer* img)
{
	unsigned char* imgByte = (unsigned char*)img->Memory;
	for(int i = 0; i < img->Height*img->Width*img->BytesPerPixel; ++i)
	{
		*imgByte = 0;
		++imgByte;
	}
}

void draw_image(ImageBuffer* Image, int x = 0, int y = 0)
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

void inline set_pixel(ImageBuffer* img, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	if(x >= img->Width) return;
	if(y >= img->Height) return;
	if(x < 0) return;
	if(y < 0) return;
	
	unsigned int* pixel = (unsigned int*)img->Memory;

	*(pixel+x+(y*img->Width)) = b | g << 8 | r << 16;
}

void inline set_pixel(ImageBuffer* img, int x, int y, Color c)
{
	set_pixel(img, x, y, c.r, c.g, c.b);
}


void line(ImageBuffer* img, int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b)
{
	bool steep = false;
	if(abs(x0-x1)<abs(y0-y1))
	{
		int temp = y1;
		y1 = x1;
		x1 = temp;
		temp = y0;
		y0 = x0;
		x0 = temp;
		steep = true;
	}

	if(x0 > x1)
	{
		int temp = y1;
		y1 = y0;
		y0 = temp;

		temp = x1;
		x1 = x0;
		x0 = temp;
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	int twiceHeight = abs(dy)*2;
	int offset = 0;

	int y = y0;

	for(int x = x0; x <= x1; ++x)
	{
		if(steep)
		{
			set_pixel(img, y, x, r, g, b);	
		} else {
			set_pixel(img, x, y, r, g, b);	
		}

		offset += twiceHeight;
		if(offset > dx)
		{
			y += (dy > 0) ? 1 : -1;
			offset -= 2*dx; 
		}
	}

}

void line(ImageBuffer* img, int x0, int y0, int x1, int y1, Color c)
{
	line(img, x0,y0,x1,y1,c.r,c.g,c.b);
}

void swap(int& i, int& j)
{
	int temp = i;
	i = j;
	j = temp; 
}

int sign(int x)
{
	return (x == 0) ? 0 : ((x < 0) ? -1 : 1);
}

int side_of_line(int x, int y, int x0, int y0, int x1, int y1)
{
	return sign((x0 - x1) * (y - y1) - (y0 - y1) * (x - x1));
}

void filled_triangle(ImageBuffer* img, int x0, int y0, int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b)
{
	if(x0 > x1)
	{
		swap(y0, y1);
		swap(x0, x1);
	}
	if(x0 > x2)
	{
		swap(y0, y2);
		swap(x0, x2);
	}
	if(x1 > x2)
	{
		swap(y1, y2);
		swap(x1, x2);
	}

	int y3 = y2 + ((x1-x2)/(x0-x2))*(y0-y2);

	int min_y = y0;
	int max_y = y0;

	if(min_y > y1) min_y = y1;
	if(min_y > y2) min_y = y2;

	if(max_y < y1) max_y = y1;
	if(max_y < y2) max_y = y2;

	bool flipped_triangle = (sign((y1 - y3)) == -1);

	for(int x = x0; x < x2; ++x)
	{
		for(int y = min_y; y < max_y; ++y)
		{
			char inside = sign((x2 - x0) * (y - y0) - (y2 - y0) * (x - x0));

			if(inside == 1 && flipped_triangle)
				break;

			inside += sign((x0 - x1) * (y - y1) - (y0 - y1) * (x - x1))
				   + sign((x1 - x2) * (y - y2) - (y1 - y2) * (x - x2));
					
			if(inside == -3)
			{
				set_pixel(img, x, y, r, g, b);
			}
			else if(inside == 3)
			{
				set_pixel(img, x, y, r, g, b);
			}
		}
	}

}

void empty_triangle(ImageBuffer* img,int x0, int y0, int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b)
{
	line(img, x0,y0,x1,y1,r,g,b);
	line(img, x1,y1,x2,y2,r,g,b);
	line(img, x2,y2,x0,y0,r,g,b);
}


void rect(ImageBuffer* img, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b)
{

	unsigned int* pixel = (unsigned int*)img->Memory;
	for(int n = 0; n < w*h; n++)
	{
		*pixel++ = b | g << 8 | r << 16;
	}
}

void rect(ImageBuffer* img, int x, int y, int w, int h, Color c)
{
	rect(img, x, y, w, h, c.r, c.g, c.b);	
}

void circle(int x, int y, int rad, unsigned char r, unsigned char g, unsigned char b){
	
}

void circle(int x, int y, int r, Color c){
	circle(x, y, r, c.r, c.g, c.b);
}


#endif