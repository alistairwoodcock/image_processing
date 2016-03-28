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
	// Width = 1920;
	// Height = 1080;
	buff = new_image_buffer(Width, Height);
}

static int i; 
static int j;
static int k = 1;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model *model = NULL;


void draw(){
	clear_image(&buff);
	
	model = new Model("obj/african_head.obj");

	// TGAImage image(Width, Height, TGAImage::RGB);
	
	// for (int i=0; i<model->nfaces(); i++) { 
	// 	std::vector<int> face = model->face(i); 
	// 	Vec2i screen_coords[3]; 
	// 	for (int j=0; j<3; j++) { 
	// 		Vec3f world_coords = model->vert(face[j]); 
	// 		screen_coords[j] = Vec2i((world_coords.x+1.)*Width/2., (world_coords.y+1.)*Height/2.); 
	// 	} 
	// 	filled_triangle(&buff, 
	// 					screen_coords[0].x, screen_coords[0].y,
	// 					screen_coords[1].x, screen_coords[1].y,
	// 					screen_coords[2].x, screen_coords[2].y,
	// 					rnd(255), rnd(255), rnd(255));
	// }

	// TGAImage image(Width, Height, TGAImage::RGB);
    Vec3f light_dir(0,0,-1);
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x+1.)*Width/2., (v.y+1.)*Height/2.);
            world_coords[j]  = v;
        }
        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n*light_dir;
        if (intensity>0) {

        	filled_triangle(&buff, 
						screen_coords[0].x, screen_coords[0].y,
						screen_coords[1].x, screen_coords[1].y,
						screen_coords[2].x, screen_coords[2].y,
						intensity*(255), intensity*(255),intensity*(255));
        }
    }

	delete model;

	flip_image_vertical(&buff);
	draw_image(&buff);
}
