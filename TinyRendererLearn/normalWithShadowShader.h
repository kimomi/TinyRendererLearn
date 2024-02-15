#pragma once
#include "tgaimage.h"
#include "basicShader.h"

struct NormalWithShadowShader : public BasicShader
{
protected:
	TGAImage* normalmap;
	TGAImage* shadowmap;

public:
	virtual BasicVertOutput* vertex(SimpleVertInput* i);
	virtual Vec4f fragment(SimpleVertOutput* i);
	virtual void setTexture(const char* textureName, TGAImage* texture);
};