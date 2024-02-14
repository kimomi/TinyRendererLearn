#include "basicShader.h"

inline BasicShader::~BasicShader()
{
	basemap = nullptr;
}

BasicShader::BasicShader() : basemap(nullptr)
{
}

void BasicShader::setTexture(const char* textureName, TGAImage* texture)
{
	if (strcmp(textureName, "basemap") == 0)
	{
		basemap = texture;
	}
	else
	{
		std::cout << "invalid property name:" << textureName << std::endl;
	}
}

void BasicShader::setVector(const char* vectorName, Vec3f vec)
{
	if (strcmp(vectorName, "lightdir") == 0)
	{
		lightdir = vec;
	}
	else
	{
		std::cout << "invalid property name:" << vectorName << std::endl;
	}
}

BasicShader::BasicVertInput* BasicShader::getVertexInput(int i, Model* model, std::vector<std::vector<int>>& face)
{
	BasicVertInput* input = new BasicVertInput();
	input->pos = model->vert(face[i][0]);
	input->uv = model->uv(face[i][1]);
	input->normal = model->normal(face[i][2]);
	return input;
}

BasicShader::BasicVertOutput* BasicShader::getFragmentProp(IShader::SimpleVertOutput** verts, Vec2i* screenPos2D, Vec2i p)
{
	BasicVertOutput** v = (BasicVertOutput**)verts;
	BasicVertOutput* o = new BasicVertOutput();
	auto bary = barycentric2D(screenPos2D, p);
	barycentricLerp(modelPos, v);
	barycentricLerp(worldPos, v);
	barycentricLerp(viewPos, v);
	barycentricLerp(clipPos, v);
	barycentricLerp(screenPos, v);
	barycentricLerp(screenPos2D, v);
	barycentricLerp(uv, v);
	barycentricLerp(normal, v);
	o->normal = o->normal.normalize();

	// compute tangent space matrix
	Vec2f deltaUV1 = v[1]->uv - v[0]->uv;
	Vec2f deltaUV2 = v[2]->uv - v[0]->uv;
	Vec3f v1 = v[1]->modelPos - v[0]->modelPos;
	Vec3f v2 = v[2]->modelPos - v[0]->modelPos;
	Matrix deltaMatrix = Matrix(2, 2);
	deltaMatrix[0][0] = deltaUV2.y;
	deltaMatrix[0][1] = -deltaUV1.y;
	deltaMatrix[1][0] = -deltaUV2.x;
	deltaMatrix[1][1] = deltaUV1.x;
	Matrix posMatrix = Matrix(2, 3);
	posMatrix[0][0] = v1.x;
	posMatrix[0][1] = v1.y;
	posMatrix[0][2] = v1.z;
	posMatrix[1][0] = v2.x;
	posMatrix[1][1] = v2.y;
	posMatrix[1][2] = v2.z;

	float d = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
	d = abs(d) < 1e-5 ? 1e-5 : d;
	Matrix tb = deltaMatrix * posMatrix * (1.0f / d);
	Vec3f t = Vec3f(tb[0][0], tb[0][1], tb[0][2]);
	Vec3f b = Vec3f(tb[1][0], tb[1][1], tb[1][2]);
	Vec3f tt = (t - (t * o->normal) * o->normal).normalize();
	Vec3f bb = (b - (b * o->normal) * o->normal - (b * tt) * tt).normalize();
	o->tangentM = Matrix();
	o->tangentM[0][0] = tt.x;
	o->tangentM[1][0] = tt.y;
	o->tangentM[2][0] = tt.z;
	o->tangentM[0][1] = bb.x;
	o->tangentM[1][1] = bb.y;
	o->tangentM[2][1] = bb.z;
	o->tangentM[0][2] = o->normal.x;
	o->tangentM[1][2] = o->normal.y;
	o->tangentM[2][2] = o->normal.z;
	o->tangentM[3][3] = 1;

	return o;
}