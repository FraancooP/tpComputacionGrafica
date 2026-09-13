#include "Mesh.h"

#include <glad/gl.h>

#include <cstddef>
#include <stdexcept>
#include <utility>

Mesh::~Mesh()
{
    clear();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vao_(std::exchange(other.vao_, 0U)),
      vbo_(std::exchange(other.vbo_, 0U)),
      ebo_(std::exchange(other.ebo_, 0U)),
      count_(std::exchange(other.count_, 0))
{
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        clear();

        vao_ = std::exchange(other.vao_, 0U);
        vbo_ = std::exchange(other.vbo_, 0U);
        ebo_ = std::exchange(other.ebo_, 0U);
        count_ = std::exchange(other.count_, 0);
    }

    return *this;
}

void Mesh::load(const MeshData& data)
{
    if (data.vertices.empty() || data.indices.empty())
    {
        throw std::invalid_argument(
            "La malla necesita vertices e indices."
        );
    }

    clear();

    glCreateVertexArrays(1, &vao_);
    glCreateBuffers(1, &vbo_);
    glCreateBuffers(1, &ebo_);

    // Copiamos los atributos de los vertices.
    glNamedBufferData(
        vbo_,
        static_cast<GLsizeiptr>(
            data.vertices.size() * sizeof(Vertex)
        ),
        data.vertices.data(),
        GL_STATIC_DRAW
    );

    // Copiamos los indices.
    glNamedBufferData(
        ebo_,
        static_cast<GLsizeiptr>(
            data.indices.size() * sizeof(unsigned int)
        ),
        data.indices.data(),
        GL_STATIC_DRAW
    );

    // Conectamos el VBO a la ranura 0 del VAO.
    glVertexArrayVertexBuffer(
        vao_,
        0,
        vbo_,
        0,
        static_cast<GLsizei>(sizeof(Vertex))
    );

    // Atributo 0: posicion.
    glVertexArrayAttribFormat(
        vao_,
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLuint>(offsetof(Vertex, px))
    );

    glVertexArrayAttribBinding(vao_, 0, 0);
    glEnableVertexArrayAttrib(vao_, 0);

    // Atributo 1: color.
    glVertexArrayAttribFormat(
        vao_,
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLuint>(offsetof(Vertex, r))
    );

    glVertexArrayAttribBinding(vao_, 1, 0);
    glEnableVertexArrayAttrib(vao_, 1);

    // El EBO tiene una conexion dedicada en el VAO.
    glVertexArrayElementBuffer(vao_, ebo_);

    count_ = static_cast<int>(data.indices.size());
}

void Mesh::clear()
{
    if (vao_ != 0)
    {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }

    if (vbo_ != 0)
    {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }

    if (ebo_ != 0)
    {
        glDeleteBuffers(1, &ebo_);
        ebo_ = 0;
    }

    count_ = 0;
}