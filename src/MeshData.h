#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};