#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include "geometry.h"

Matrix::Matrix(int r, int c) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i=0; i<dimensions; i++) {
        for (int j=0; j<dimensions; j++) {
            E[i][j] = (i==j ? 1.f : 0.f);
        }
    }
    return E;
}

std::vector<float>& Matrix::operator[](const int i) {
    assert(i>=0 && i<rows);
    return m[i];
}

Vec3f Matrix::operator*(const Vec3f v)
{
    float w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + m[3][3];
    Vec3f result = Vec3f(
        v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3],
        v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3],
        v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3]
    );
    result = result * (1.0f / w);
    return result;
}

Matrix Matrix::operator*(const Matrix& a) {
    assert(cols == a.rows);
    Matrix result(rows, a.cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<a.cols; j++) {
            result.m[i][j] = 0.f;
            for (int k=0; k<cols; k++) {
                result.m[i][j] += m[i][k]*a.m[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator*(const float a) {
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.m[i][j] = m[i][j] * a;
        }
    }
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[j][i] = m[i][j];
    return result;
}

Matrix Matrix::inverse() {
    assert(rows==cols);
    // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
    Matrix result(rows, cols*2);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[i][j] = m[i][j];
    for(int i=0; i<rows; i++)
        result[i][i+cols] = 1;
    // first pass
    for (int i=0; i<rows-1; i++) {
        // normalize the first row
        for(int j=result.cols-1; j>=0; j--)
            result[i][j] /= result[i][i];
        for (int k=i+1; k<rows; k++) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // normalize the last row
    for(int j=result.cols-1; j>=rows-1; j--)
        result[rows-1][j] /= result[rows-1][rows-1];
    // second pass
    for (int i=rows-1; i>0; i--) {
        for (int k=i-1; k>=0; k--) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate(rows, cols);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            truncate[i][j] = result[i][j+cols];
    return truncate;
}

std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i=0; i<m.nrows(); i++)  {
        for (int j=0; j<m.ncols(); j++) {
            s << m[i][j];
            if (j<m.ncols()-1) s << "\t";
        }
        s << "\n";
    }
    return s;
}

float cross(Vec2i a, Vec2i b)
{
    return a.x * b.y - a.y * b.x;
}

Vec3f cross(const Vec3f& v1, const Vec3f& v2) {
    return Vec3f(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
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

Matrix viewport(int x, int y, int w, int h, int depth) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}

Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye - center).normalize();
    Vec3f x = cross(up, z).normalize();
    Vec3f y = cross(z, x).normalize();
    Matrix Minv = Matrix::identity(4);
    Matrix Tr = Matrix::identity(4);
    for (int i = 0; i < 3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -eye[i];
    }
    return Minv * Tr;
}