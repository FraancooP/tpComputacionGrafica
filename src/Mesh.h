#pragma once

#include "MeshData.h"

class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    // Cada malla es propietaria de sus recursos.
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // Esta version trabaja con mallas indexadas.
    void load(const MeshData& data);

    // Libera los recursos y deja la malla vacia.
    void clear();

    unsigned int vao() const
    {
        return vao_;
    }

    // Cantidad de indices, no de vertices.
    int count() const
    {
        return count_;
    }

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int ebo_ = 0;

    int count_ = 0;
};