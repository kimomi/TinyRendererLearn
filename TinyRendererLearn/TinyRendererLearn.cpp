// TinyRendererLearn.cpp: 定义应用程序的入口点。
//

#include "TinyRendererLearn.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const Vec3f lightdir = Vec3f(0.0, 0.0, -1.0).normalize();
const Vec3f camera(0, 0, 3);
const int width = 512;
const int height = 512;
const int depth = 255;

Matrix viewport(int x, int y, int w, int h) {
	Matrix m = Matrix::identity(4);
	m[0][3] = x + w / 2.f;
	m[1][3] = y + h / 2.f;
	m[2][3] = depth / 2.f;

	m[0][0] = w / 2.f;
	m[1][1] = h / 2.f;
	m[2][2] = depth / 2.f;
	return m;
}


int main(int argc, char** argv)
{
	TGAImage image(width, height, TGAImage::RGB);
	//image.fill(white);

	float* zbuffer = new float[width * height];
	for (size_t i = 0, c = width * height; i < c; i++)
	{
		zbuffer[i] = -FLT_MAX;
	}

    Model model("../../../obj/african_head.obj");
	TGAImage basemap;
	basemap.read_tga_file("../../../texture/african_head_diffuse.tga");
	basemap.flip_vertically();

	Vec3i screenPos[3];
	Vec2i screenPos2D[3];
	Vec2f vertUvs[3];
	Vec3f modelPos[3];
	Vec3f worldPos[3];
	Vec3f viewPos[3];
	Vec3f clipPos[3];
	Vec3f normal;

	Matrix modelM = Matrix::identity(4);
	Matrix viewM = Matrix::identity(4);
	Matrix projectionM = Matrix::identity(4);
	projectionM[3][2] = -1.0f / camera.z;
	Matrix clip2screenM = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);

	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<std::vector<int>> face = model.face(i);

		// 三角形顶点数据
		for (size_t i = 0; i < 3; i++)
		{
			modelPos[i] = model.vert(face[i][0]);
			vertUvs[i] = model.uv(face[i][1]);
			worldPos[i] = modelM * modelPos[i];
			viewPos[i] = viewM * worldPos[i];
			clipPos[i] = projectionM * viewPos[i];
			auto screenPosTemp = clip2screenM * clipPos[i];
			screenPos[i] = Vec3i((int)screenPosTemp.x, (int)screenPosTemp.y, (int)screenPosTemp.z);
			screenPos2D[i] = Vec2i(screenPos[i].x, screenPos[i].y);
		}
		Vec3f n = (worldPos[2] - worldPos[0]) ^ (worldPos[1] - worldPos[0]);
		n.normalize();


		int xmin = min(screenPos[0].x, min(screenPos[1].x, screenPos[2].x));
		int xmax = max(screenPos[0].x, max(screenPos[1].x, screenPos[2].x));
		int ymin = min(screenPos[0].y, min(screenPos[1].y, screenPos[2].y));
		int ymax = max(screenPos[0].y, max(screenPos[1].y, screenPos[2].y));
		Vec2i p;
		for (int x = xmin; x < xmax; x++)
		{
			for (int y = ymin; y < ymax; y++)
			{
				p.x = x;
				p.y = y;
				if (x >= 0 && x < width && y >= 0 && y < height && isInTriangle(screenPos2D, p))
				{
					auto bary = barycentric2D(screenPos2D, p);
					float z = bary.x * worldPos[0].z + bary.y * worldPos[1].z + bary.z * worldPos[2].z;
					// Z TEST
					if (z > zbuffer[x + y * width])
					{
						zbuffer[x + y * width] = z;

						// init args
						Vec2f uv = vertUvs[0] * bary.x + vertUvs[1] * bary.y + vertUvs[2] * bary.z;

						// ambient
						auto baseTGAColor = basemap.get(uv.x * basemap.get_width(), uv.y * basemap.get_height());
						auto baseColor = Vec3f((float)baseTGAColor.r / 255, (float)baseTGAColor.g / 255, (float)baseTGAColor.b / 255);
						baseColor = baseColor * 0.9f;

						// diffuse
						float diffuseRatio = n * lightdir;			
						float gray = clamp(diffuseRatio, 0.0f, 1.0f) * 0.0f;
						Vec3f diffuseColor = Vec3f(gray, gray, gray);
						Vec3f color = baseColor + diffuseColor;

						color.x = clamp(color.x, 0.0f, 1.0f);
						color.y = clamp(color.y, 0.0f, 1.0f);
						color.z = clamp(color.z, 0.0f, 1.0f);
						TGAColor outTGAColor = TGAColor((int)(color.x * 255), (int)(color.y * 255), (int)(color.z * 255), 255);


						image.set(x, y, outTGAColor);
					}
				}
			}
		}
	}
	//drawModelWire(&model, image, width, height, white);
	delete[] zbuffer;

	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}