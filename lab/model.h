#pragma once
#include <vector>
#include <string>

struct Vertex {
    float x, y, z;
};

extern std::vector<Vertex> vertices;
extern std::vector<unsigned int> indices;

bool loadModel(const std::string& filename);

