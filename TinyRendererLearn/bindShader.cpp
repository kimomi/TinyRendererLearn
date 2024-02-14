#include "bindShader.h"

BasicShader::BasicVertOutput* BindShader::vertex(IShader::SimpleVertInput* basei)
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

Vec4f BindShader::fragment(IShader::SimpleVertOutput* basei)
{
	BasicShader::BasicVertOutput* i = (BasicShader::BasicVertOutput*)basei;

	float diffuseRatio = i->normal * (Vec3f(0.0f, 0.0f, 0.0f) - lightdir);
	float gray = clamp(diffuseRatio, 0.0f, 1.0f);
	Vec3f color = Vec3f(1.0, 0.9, 0.1) * (floor(gray * 5) / 5.0f);
	return Vec4f(color.x, color.y, color.z, 1.0f);
}