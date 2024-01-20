// TinyRendererLearn.cpp: 定义应用程序的入口点。
//

#include "TinyRendererLearn.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char** argv)
{
    int width = 512;
    int height = 512;
	TGAImage image(width, height, TGAImage::RGB);

	//for (size_t i = 0; i < width; i++)
	//{
	//	for (size_t j = 0; j < height; j++)
	//	{
	//		image.set(i, j, white);
	//	}
	//}

    Model model("../../../obj/african_head.obj");
    //drawModelWire(&model, image, width, height, white);


	Vec3f lightdir = Vec3f(0.0, 0.0, -1.0).normalize();

	Vec2i pts[3];
	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		Vec3f v0 = model.vert(face[0]);
		Vec3f v1 = model.vert(face[1]);
		Vec3f v2 = model.vert(face[2]);
		Vec3f n = (v2 - v0) ^ (v1 - v0);
		n.normalize();

		float diffuseRatio = n * lightdir;

		if (diffuseRatio <= 0)
		{
			continue;
		}

		unsigned char gray = clamp((int)floor(10 + diffuseRatio * 255), 0, 255);
		TGAColor color = TGAColor(gray, gray, gray, 255);
		//TGAColor color = TGAColor(n.x * 255, n.y * 255, n.z * 255, 255);

		int x0 = (v0.x + 1.) / 2. * width;
		int y0 = (v0.y + 1.) / 2. * height;
		int x1 = (v1.x + 1.) / 2. * width;
		int y1 = (v1.y + 1.) / 2. * height;
		int x2 = (v2.x + 1.) / 2. * width;
		int y2 = (v2.y + 1.) / 2. * height;
		pts[0] = Vec2i(x0, y0);
		pts[1] = Vec2i(x1, y1);
		pts[2] = Vec2i(x2, y2);
		drawTriangle(pts, image, color);
	}

	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}