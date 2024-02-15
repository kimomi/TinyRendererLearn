#include "shadowMapShader.h"

ShadowMapShader::BasicVertOutput* ShadowMapShader::vertex(IShader::SimpleVertInput* basei)
{
	ShadowMapShader::BasicVertInput* i = (ShadowMapShader::BasicVertInput*)basei;
	BasicVertOutput* o = new BasicVertOutput();
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

Vec4f ShadowMapShader::fragment(IShader::SimpleVertOutput* basei)
{
	ShadowMapShader::BasicVertOutput* i = (ShadowMapShader::BasicVertOutput*)basei;
	float depthColor = packZ2Color(i->screenPos.z);
	return Vec4f(depthColor, depthColor, depthColor, 1.0f) ;
}