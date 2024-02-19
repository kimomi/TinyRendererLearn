#pragma once
#include "geometry.h"
#include "shader.h"
#include "tgaimage.h"

class PostProcessShader
{
private:
	TGAImage* src;
	float* zbuffer;
	Vec2i iResolution;

public:
	void init(TGAImage* src, float* zbuffer, Vec2i resolution);
	Vec4f fragment(Vec2i fragCoord);
};