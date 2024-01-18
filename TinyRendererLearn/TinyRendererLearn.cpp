// TinyRendererLearn.cpp: 定义应用程序的入口点。
//

#include "TinyRendererLearn.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void drawWire(Model* model, TGAImage& image, int width, int height)
{
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            // [-1, 1] -> [0, width]
            int x0 = (v0.x + 1.) / 2. * width;
            int y0 = (v0.y + 1.) / 2. * height;
            int x1 = (v1.x + 1.) / 2. * width;
            int y1 = (v1.y + 1.) / 2. * height;
            line(x0, y0, x1, y1, image, white);
        }
    }
}

int main(int argc, char** argv)
{
    int width = 512;
    int height = 512;
	TGAImage image(width, height, TGAImage::RGB);

	//line(13, 20, 80, 40, image, white);
	//line(20, 13, 40, 80, image, red);
	//line(80, 40, 13, 20, image, red);

    Model model("D:\\Develop\\CPlusPlus\\TinyRendererLearn\\TinyRendererLearn\\obj\\african_head.obj");
    drawWire(&model, image, width, height);

	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}