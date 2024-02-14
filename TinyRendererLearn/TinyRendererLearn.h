// TinyRendererLearn.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#include "tgaimage.h"
#include "model.h"

using namespace std;

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void drawModelWire(Model* model, TGAImage& image, int width, int height, TGAColor color)
{
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<std::vector<int> > face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j][0]);
            Vec3f v1 = model->vert(face[(j + 1) % 3][0]);
            // [-1, 1] -> [0, width]
            int x0 = (v0.x + 1.) / 2. * width;
            int y0 = (v0.y + 1.) / 2. * height;
            int x1 = (v1.x + 1.) / 2. * width;
            int y1 = (v1.y + 1.) / 2. * height;
            line(x0, y0, x1, y1, image, color);
        }
    }
}

void drawTriangle(Vec2i* pts, TGAImage& image, TGAColor color)
{
    int xmin = min(pts[0].x, min(pts[1].x, pts[2].x));
    int xmax = max(pts[0].x, max(pts[1].x, pts[2].x));
    int ymin = min(pts[0].y, min(pts[1].y, pts[2].y));
    int ymax = max(pts[0].y, max(pts[1].y, pts[2].y));

    Vec2i ab = pts[1] - pts[0];
    Vec2i bc = pts[2] - pts[1];
    Vec2i ca = pts[0] - pts[2];

    Vec2i p;
    for (int x = xmin; x < xmax; x++)
    {
        for (int y = ymin; y < ymax; y++)
        {
            p.x = x;
            p.y = y;
            if (isInTriangle(pts, p))
            {
                image.set(x, y, color);
            }
        }
    }
}

