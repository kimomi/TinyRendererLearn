#include "normalWithShadowShader.h"

BasicShader::BasicVertOutput* NormalWithShadowShader::vertex(IShader::SimpleVertInput* basei)
{
	BasicShader::BasicVertInput* i = (BasicShader::BasicVertInput*)basei;
	BasicShader::BasicVertOutput* o = new BasicShader::BasicVertOutput();
	o->modelPos = i->pos;
	o->uv = i->uv;
	o->normal = i->normal;
	o->worldPos = modelM * o->modelPos;
	o->viewPos = viewM * o->worldPos;
	o->clipPos = projectionM * o->viewPos;
	o->screenPos = clip2screenM * o->clipPos;
	o->screenPos2D = Vec2i((int)(o->screenPos.x), (int)(o->screenPos.y));
	return o;
}

Vec4f NormalWithShadowShader::fragment(IShader::SimpleVertOutput* basei)
{
	NormalWithShadowShader::BasicVertOutput* i = (NormalWithShadowShader::BasicVertOutput*)basei;

	auto baseTGAColor = basemap->getByUv(i->uv.x, i->uv.y);
	auto baseColor = Vec3f((float)baseTGAColor.r / 255, (float)baseTGAColor.g / 255, (float)baseTGAColor.b / 255);

	// ambient
	auto ambientColor = Vec3f(0.2f, 0.2f, 0.2f);

	// normal from normal map
	auto normalTGAColor = normalmap->getByUv(i->uv.x, i->uv.y);
	auto normalColor = Vec3f((float)normalTGAColor.r / 255, (float)normalTGAColor.g / 255, (float)normalTGAColor.b / 255);
	// normal color to dir
	auto normaldir = normalColor * 2.0 - Vec3f(1.0, 1.0, 1.0);
	auto n = modelM * i->tangentM * normaldir;

	// diffuse
	float diffuseRatio = n * (Vec3f(0.0f, 0.0f, 0.0f) - lightdir);
	diffuseRatio = clamp(diffuseRatio, 0.0f, 1.0f);
	auto diffuseColor = Vec3f(0.9, 0.9, 0.8) * diffuseRatio;

	// shadow
	float shadowRatio = 1.0f;
	auto shadowMapSamplePos = world2shadowmapM * i->worldPos;
	float curShadowZColor = packZ2Color(shadowMapSamplePos.z);
	if (curShadowZColor + 1e3 < (shadowmap->get(int(shadowMapSamplePos.x), int(shadowMapSamplePos.y)).r / 255.0f))
	{
		shadowRatio = 0.1f;
	}

	// combine color
	Vec3f color = baseColor.mul(ambientColor + diffuseColor) * shadowRatio;

	return Vec4f(color.x, color.y, color.z, 1.0f);
}

void NormalWithShadowShader::setTexture(const char* textureName, TGAImage* texture)
{
	if (strcmp(textureName, "basemap") == 0)
	{
		basemap = texture;
	}
	else if (strcmp(textureName, "normalmap") == 0)
	{
		normalmap = texture;
	}
	else if (strcmp(textureName, "shadowmap") == 0)
	{
		shadowmap = texture;
	}
	else
	{
		std::cout << "invalid property name:" << textureName << std::endl;
	}
}
