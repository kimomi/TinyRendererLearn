#include "TinyRendererLearn.h"
#include "gouraudShader.h"
#include "bindShader.h"
#include "normalMappingShader.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const Vec3f lightdir = Vec3f(0.0, 0.0, -1.0).normalize();
Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);
const int width = 1024;
const int height = 1024;
const int depth = 255;

Model* model;
float* zbuffer;
TGAImage* renderTarget;
TGAImage* basemap;
TGAImage* shadermap;

void innerRender(IShader& shader, Model* model, TGAImage* renderTarget, float* zbuffer)
{
    int width = renderTarget->get_width();
    int height = renderTarget->get_height();

    IShader::SimpleVertInput* vertInput[3];
    IShader::SimpleVertOutput* vertOutput[3];
    Vec2i screenPos2D[3];
    Vec2i p;
    for (int i = 0; i < model->nfaces(); i++)
    {
        // triangle vert props
        auto face = model->face(i);
        for (size_t j = 0; j < 3; j++)
        {
            vertInput[j] = shader.getVertexInput(j, model, face);
            vertOutput[j] = shader.vertex(vertInput[j]);
        }

        // draw triangle
        screenPos2D[0] = Vec2i(vertOutput[0]->screenPos.x, vertOutput[0]->screenPos.y);
        screenPos2D[1] = Vec2i(vertOutput[1]->screenPos.x, vertOutput[1]->screenPos.y);
        screenPos2D[2] = Vec2i(vertOutput[2]->screenPos.x, vertOutput[2]->screenPos.y);
        int xmin = min(screenPos2D[0].x, min(screenPos2D[1].x, screenPos2D[2].x));
        int xmax = max(screenPos2D[0].x, max(screenPos2D[1].x, screenPos2D[2].x));
        int ymin = min(screenPos2D[0].y, min(screenPos2D[1].y, screenPos2D[2].y));
        int ymax = max(screenPos2D[0].y, max(screenPos2D[1].y, screenPos2D[2].y));
        for (int x = xmin; x < xmax; x++)
        {
            for (int y = ymin; y < ymax; y++)
            {
                p.x = x;
                p.y = y;
                if (x >= 0 && x < width && y >= 0 && y < height && isInTriangle(screenPos2D, p))
                {
                    auto fragmentInput = shader.getFragmentProp(vertOutput, screenPos2D, p);
                    // Z TEST
                    if (fragmentInput -> screenPos.z > zbuffer[x + y * width])
                    {
                        zbuffer[x + y * width] = fragmentInput -> screenPos.z;

                        // fragment
                        auto color = shader.fragment(fragmentInput);

                        // set output image color
                        color.x = clamp(color.x, 0.0f, 1.0f);
                        color.y = clamp(color.y, 0.0f, 1.0f);
                        color.z = clamp(color.z, 0.0f, 1.0f);
                        TGAColor outTGAColor = TGAColor((int)(color.x * 255), (int)(color.y * 255), (int)(color.z * 255), (int)(color.w * 255));
                        renderTarget->set(x, y, outTGAColor);
                    }
                    delete fragmentInput;
                }
            }
        }
        for (size_t j = 0; j < 3; j++)
        {
            delete vertInput[j];
            delete vertOutput[j];
        }
    }
}

void render()
{
	NormalMappingShader shader;

	auto modelM = Matrix::identity(4);
	auto viewM = lookat(eye, center, Vec3f(0.0, 1.0, 0.0));
	auto projectionM = Matrix::identity(4);
	projectionM[3][2] = -1.0f / (eye - center).norm();
	auto clip2screenM = viewport(0, 0, width, height, depth);
	shader.initMartix(modelM, viewM, projectionM, clip2screenM);

	shader.setTexture("basemap", basemap);
    shader.setTexture("normalmap", shadermap);
	shader.setVector("lightdir", lightdir);

	innerRender(shader, model, renderTarget, zbuffer);
}

void initRenderState(const char* modelName)
{
	model = new Model(modelName);
	zbuffer = new float[width * height];
	for (size_t i = 0, c = width * height; i < c; i++)
	{
		zbuffer[i] = -FLT_MAX;
	}

	renderTarget = new TGAImage(width, height, TGAImage::RGB);
	basemap = new TGAImage();
	basemap->read_tga_file("../../../texture/african_head_diffuse.tga");
	basemap->flip_vertically();

    shadermap = new TGAImage();
    shadermap->read_tga_file("../../../texture/african_head_nm_tangent.tga");
    shadermap->flip_vertically();
}

void showRenderResult()
{
	renderTarget -> flip_vertically();
	renderTarget -> write_tga_file("output.tga");
}

void clearRenderState()
{
	delete model;
	delete[] zbuffer;
	delete renderTarget;
	delete basemap;
    delete shadermap;
}

int main(int argc, char** argv)
{
	const char* modelName = "../../../obj/african_head.obj";
	if (argc >= 2) 
	{
		modelName = argv[1];
	}
	initRenderState(modelName);

	render();

	showRenderResult();
	clearRenderState();
	return 0;
}
