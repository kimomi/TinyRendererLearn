#pragma once
#include "tgaimage.h"
#include "basicShader.h"

struct GouraudShader : public BasicShader
{
public:
	virtual BasicVertOutput* vertex(SimpleVertInput* i);
	virtual Vec4f fragment(SimpleVertOutput* i);
};