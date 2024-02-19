#include "postProcessShader.h"
#include "geometry.h"

void PostProcessShader::init(TGAImage* src, float* zbuffer, Vec2i resolution)
{
	this->src = src;
	this->zbuffer = zbuffer;
	this->iResolution = resolution;
}

Vec4f PostProcessShader::fragment(Vec2i fragCoord)
{
	auto srcTGAColor = src->get(fragCoord.x, fragCoord.y);
	auto srcColor = Vec3f(srcTGAColor.r / 255.0f, srcTGAColor.g / 255.0f, srcTGAColor.b / 255.0f);

	// ao
	float baseColor = 0.0f;
	int size = 5;
	float a = 1.0f / ((2 * size + 1) * (2 * size + 1));
	float z = zbuffer[fragCoord.x + fragCoord.y * iResolution.y];
	for (int x = fragCoord.x - size; x <= fragCoord.x + size; x++)
	{
		for (int y = fragCoord.y - size; y <= fragCoord.y + size; y++)
		{
			if (x >= 0 && y >= 0 && x < iResolution.x && y < iResolution.y && zbuffer[x + y * iResolution.y] < z)
			{
				baseColor += a;
			}
		}
	}

	baseColor = 2.0f * clamp(baseColor, 0.0f, 0.5f);
	//return Vec4f(baseColor, baseColor, baseColor, 1.0f);
	auto color = srcColor * baseColor;
	return Vec4f(color.x, color.y, color.z, 1.0f);
}
