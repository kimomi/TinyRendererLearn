// TinyRendererLearn.cpp: 定义应用程序的入口点。
//

#include "TinyRendererLearn.h"
#include "tgaimage.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char** argv)
{
	TGAImage imge(128, 128, TGAImage::RGB);
	for (int x = 0; x < 128; x++)
	{
		for (int y = 0; y < 128; y++)
		{
			imge.set(x, y, white);
		}
	}
	for (int x = 10; x < 20; x++)
	{
		for (int y = 80; y < 90; y++)
		{
			imge.set(x, y, red);
		}
	}
	imge.flip_vertically();
	imge.write_tga_file("output.tga");

	return 0;
}