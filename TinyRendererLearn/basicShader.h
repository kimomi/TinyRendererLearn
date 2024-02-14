#pragma once
#include "tgaimage.h"
#include "shader.h"

struct BasicShader : public IShader
{
protected:
	TGAImage* basemap;
	Vec3f lightdir;

public:
	struct BasicVertInput : SimpleVertInput
	{
		Vec2f uv;
		Vec3f normal;
	};

	struct BasicVertOutput : SimpleVertOutput
	{
		Vec3f modelPos;
		Vec3f worldPos;
		Vec3f viewPos;
		Vec3f clipPos;
		Vec2i screenPos2D;
		Vec2f uv;
		Vec3f normal;
		Matrix tangentM;
	};

	virtual ~BasicShader();
	BasicShader();
	virtual void setTexture(const char* textureName, TGAImage* texture);
	void setVector(const char* vectorName, Vec3f vec);
	virtual BasicVertInput* getVertexInput(int i, Model* model, std::vector<std::vector<int>>& face);
	virtual BasicVertOutput* getFragmentProp(SimpleVertOutput** verts, Vec2i* screenPos2D, Vec2i p);
	virtual BasicVertOutput* vertex(SimpleVertInput* i) = 0;
	virtual Vec4f fragment(SimpleVertOutput* i) = 0;
};