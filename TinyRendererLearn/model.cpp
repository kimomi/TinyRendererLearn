#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) : verts_(), faces_(), uvs_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v.raw[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f v;
            for (int i = 0; i < 2; i++)
            {
                iss >> v.raw[i];
            }
            uvs_.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<std::vector<int>> f;
            int itrash, idx, uvIndex, normalIndex;
            iss >> trash;
            while (iss >> idx >> trash >> uvIndex >> trash >> normalIndex) {
                idx--; // in wavefront obj all indices start at 1, not zero
                uvIndex--;
                normalIndex--;
                std::vector<int> inf;
                inf.push_back(idx);
                inf.push_back(uvIndex);
                inf.push_back(normalIndex);
                f.push_back(inf);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << "# vt# " << uvs_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<std::vector<int>> Model::face(int idx) {
    return faces_[idx];
}

Vec2f Model::uv(int i) {
    return uvs_[i];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}
