#include "drawing.h"
#include <vector>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

ImageBuffer buff = {};

Color c1, c2 = {};

void setup(){
	// draw_once = true;
	c2.r = 255;
	c1.b = 255;
	Width = 1920;
	Height = 1080;
	buff = new_image_buffer(Width, Height);
}

static int i; 
static int j;

void draw(){
	clear_image(&buff);
	
	model = new Model("obj/african_head.obj");
    
    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j=0; j<3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            line(&buff, x0, y0, x1, y1, 255, 255, 255);
        }
    }

    // image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    // image.write_tga_file("output.tga");
    
    delete model;
    
	draw_image(&buff);
}
