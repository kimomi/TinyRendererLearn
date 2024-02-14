#include "gouraudShader.h"

GouraudShader::BasicVertOutput* GouraudShader::vertex(IShader::SimpleVertInput* basei)
{
	GouraudShader::BasicVertInput* i = (GouraudShader::BasicVertInput*)basei;
	BasicVertOutput* o = new BasicVertOutput();
	o->modelPos = i -> pos;
	o->uv = i -> uv;
	o->normal = i -> normal;
	o->worldPos = modelM * o->modelPos;
	o->viewPos = viewM * o->worldPos;
	o->clipPos = projectionM * o->viewPos;
	o->screenPos = clip2screenM * o->clipPos;
	o->screenPos2D = Vec2i((int)(o->screenPos.x), (int)(o->screenPos.y));
	return o;
}

Vec4f GouraudShader::fragment(IShader::SimpleVertOutput* basei)
{
	GouraudShader::BasicVertOutput* i = (GouraudShader::BasicVertOutput*)basei;

	// ambient
	auto baseTGAColor = basemap->getByUv(i->uv.x, i->uv.y);
	auto baseColor = Vec3f((float)baseTGAColor.r / 255, (float)baseTGAColor.g / 255, (float)baseTGAColor.b / 255);
	baseColor = baseColor * 0.9f;

	// diffuse
	float diffuseRatio = i->normal * (Vec3f(0.0f, 0.0f, 0.0f) - lightdir);
	float gray = clamp(diffuseRatio, 0.0f, 1.0f) * 1.0f;
	Vec3f diffuseColor = Vec3f(gray, gray, gray);

	Vec3f color = baseColor;

	return Vec4f(color.x, color.y, color.z, 1.0f);
}