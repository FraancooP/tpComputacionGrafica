#pragma once

#include <vector>

struct Vertex
{
    float px, py, pz;
    float r, g, b;
};

struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};