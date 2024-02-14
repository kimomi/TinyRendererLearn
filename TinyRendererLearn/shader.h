#pragma once
#include "model.h"
#include "geometry.h"
#define barycentricLerp(__PROP__, verts) o->__PROP__ = bary.x * verts[0]->__PROP__ + bary.y * verts[1]->__PROP__ + bary.z * verts[2]->__PROP__

struct IShader
{
protected:
	Matrix modelM;
	Matrix viewM;
	Matrix projectionM;
	Matrix clip2screenM;

public:
	struct SimpleVertInput
	{
		Vec3f pos;
	};

	struct SimpleVertOutput
	{
		Vec3f screenPos;
	};

	~IShader() {};
	virtual SimpleVertOutput* vertex(SimpleVertInput* i) = 0;
	virtual Vec4f fragment(SimpleVertOutput* i) = 0;
	void initMartix(Matrix m, Matrix v, Matrix p, Matrix c)
	{
		modelM = m;
		viewM = v;
		projectionM = p;
		clip2screenM = c;
	}
	virtual SimpleVertInput* getVertexInput(int i, Model* model, std::vector<std::vector<int>>& face) = 0;
	virtual SimpleVertOutput* getFragmentProp(SimpleVertOutput** verts, Vec2i* screenPos2D, Vec2i p) = 0;
};