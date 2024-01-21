// TinyRendererLearn.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <iostream>
using namespace std;

// TODO: 在此处引用程序需要的其他标头。

float cross(Vec2i a, Vec2i b)
{
    return a.x * b.y - a.y * b.x;
}

Vec3f cross(const Vec3f& v1, const Vec3f& v2) {
    return Vec3f(v1.y* v2.z - v1.z * v2.y, v1.z* v2.x - v1.x * v2.z, v1.x* v2.y - v1.y * v2.x);
}

template <typename T>
T clamp(T x, T a, T b)
{
    if (x < a)
    {
        return a;
    }

    if (x > b)
    {
        return b;
    }

    return x;
}

bool isInTriangle(Vec2i* pts, Vec2i p)
{
    Vec2i ab = pts[1] - pts[0];
    Vec2i bc = pts[2] - pts[1];
    Vec2i ca = pts[0] - pts[2];

    bool side0 = cross(p - pts[0], ab) > 0;
    bool side1 = cross(p - pts[1], bc) > 0;
    bool side2 = cross(p - pts[2], ca) > 0;

    return side0 == side1 && side0 == side2;
}

Vec3f barycentric2D(Vec2i* pts, Vec2i P)
{
    auto A = pts[0];
    auto B = pts[1];
    auto C = pts[2];

    double det = (B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y);
    if (det == 0)
    {
        det = 0.00001;
    }
    double factor_alpha = (B.y - C.y) * (P.x - C.x) + (C.x - B.x) * (P.y - C.y);
    double factor_beta = (C.y - A.y) * (P.x - C.x) + (A.x - C.x) * (P.y - C.y);
    double alpha = factor_alpha / det;
    double beta = factor_beta / det;
    double gamma = 1.0 - alpha - beta;
    return Vec3f(alpha, beta, gamma);
}


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

