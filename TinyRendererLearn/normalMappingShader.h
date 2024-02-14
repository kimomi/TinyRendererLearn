#pragma once
#include "tgaimage.h"
#include "basicShader.h"

struct NormalMappingShader : public BasicShader
{
protected:
	TGAImage* normalmap;

public:
	virtual BasicVertOutput* vertex(SimpleVertInput* i);
	virtual Vec4f fragment(SimpleVertOutput* i);
	virtual void setTexture(const char* textureName, TGAImage* texture);
};