#include "TinyRendererLearn.h"
#include "gouraudShader.h"
#include "bindShader.h"
#include "normalMappingShader.h"
#include "shadowMapShader.h"
#include "normalWithShadowShader.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const Vec3f lightdir = Vec3f(0.5, 0.0, -1.0).normalize();
Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);
const int width = 1024;
const int height = 1024;
const int depth = 255;

Model* model;
float* zbuffer;
TGAImage* renderTarget;
TGAImage* basemap;
TGAImage* normalmap;

TGAImage* shadowmap;

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

void resetRenderState()
{
    for (size_t i = 0, c = width * height; i < c; i++)
    {
        zbuffer[i] = -FLT_MAX;
    }
}

Matrix renderShadow()
{
    resetRenderState();
    ShadowMapShader shader;

    Vec3f lightPos = lightdir * -2.0f;
    auto modelM = Matrix::identity(4);
    auto viewM = lookat(lightPos, Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0, 1.0, 0.0));
    auto projectionM = Matrix::identity(4);
    auto clip2screenM = viewport(0.0f, 0.0f, width, height, depth);
    shader.initMartix(modelM, viewM, projectionM, clip2screenM);
    innerRender(shader, model, shadowmap, zbuffer);
    return clip2screenM * projectionM * viewM;
}


void render()
{
    auto world2shadowmapM = renderShadow();

    resetRenderState();
	NormalWithShadowShader shader;

	auto modelM = Matrix::identity(4);
	auto viewM = lookat(eye, center, Vec3f(0.0, 1.0, 0.0));
	auto projectionM = Matrix::identity(4);
	projectionM[3][2] = -1.0f / (eye - center).norm();
	auto clip2screenM = viewport(0.0f, 0.0f, width, height, depth);
	shader.initMartix(modelM, viewM, projectionM, clip2screenM, world2shadowmapM);

	shader.setTexture("basemap", basemap);
    shader.setTexture("normalmap", normalmap);
	shader.setVector("lightdir", lightdir);
    shader.setTexture("shadowmap", shadowmap);

	innerRender(shader, model, renderTarget, zbuffer);
}


void initRenderState()
{
	//model = new Model("../../../obj/african_head.obj");
    model = new Model("../../../obj/diablo3_pose.obj");
	zbuffer = new float[width * height];

	renderTarget = new TGAImage(width, height, TGAImage::RGB);
	basemap = new TGAImage();
	//basemap->read_tga_file("../../../texture/african_head_diffuse.tga");
    basemap->read_tga_file("../../../texture/diablo3_pose_diffuse.tga");
	basemap->flip_vertically();

    normalmap = new TGAImage();
    //normalmap->read_tga_file("../../../texture/african_head_nm_tangent.tga");
    normalmap->read_tga_file("../../../texture/diablo3_pose_nm_tangent.tga");
    normalmap->flip_vertically();

    shadowmap = new TGAImage(width, height, TGAImage::RGB);
}

void showRenderResult()
{
	renderTarget -> flip_vertically();
	renderTarget -> write_tga_file("output.tga");

    shadowmap->flip_vertically();
    shadowmap->write_tga_file("shadowmap.tga");
}

void clearRenderState()
{
	delete model;
	delete[] zbuffer;
	delete renderTarget;
	delete basemap;
    delete normalmap;
    delete shadowmap;
}

int main(int argc, char** argv)
{
	initRenderState();

	render();

	showRenderResult();
	clearRenderState();
	return 0;
}
