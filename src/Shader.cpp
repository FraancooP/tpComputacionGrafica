#include "Shader.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <utility>

namespace
{
    GLuint compilarEtapa(
        GLenum tipo,
        const std::string &fuente)
    {
        GLuint shader = glCreateShader(tipo);

        const char *texto = fuente.c_str();

        glShaderSource(shader, 1, &texto, nullptr);
        glCompileShader(shader);

        GLint correcto = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &correcto);

        if (correcto != GL_TRUE)
        {
            const char *nombre =
                tipo == GL_VERTEX_SHADER ? "vertex" : "fragment";

            std::cerr
                << "Error al compilar el "
                << nombre
                << " shader:\n";

            GLint longitud = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &longitud);

            if (longitud > 0)
            {
                std::string log(
                    static_cast<std::size_t>(longitud),
                    '\0');

                glGetShaderInfoLog(
                    shader,
                    longitud,
                    nullptr,
                    log.data());

                std::cerr << log << '\n';
            }

            glDeleteShader(shader);

            return 0;
        }

        return shader;
    }
}

Shader::~Shader()
{
    clear();
}

Shader::Shader(Shader &&other) noexcept
    : id_(std::exchange(other.id_, 0U))
{
}

Shader &Shader::operator=(Shader &&other) noexcept
{
    if (this != &other)
    {
        clear();
        id_ = std::exchange(other.id_, 0U);
    }

    return *this;
}

bool Shader::compile_from_source(
    const std::string &vs,
    const std::string &fs)
{
    clear();

    GLuint vertexShader = compilarEtapa(GL_VERTEX_SHADER, vs);

    if (vertexShader == 0)
    {
        return false;
    }

    GLuint fragmentShader = compilarEtapa(GL_FRAGMENT_SHADER, fs);

    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return false;
    }

    id_ = glCreateProgram();

    glAttachShader(id_, vertexShader);
    glAttachShader(id_, fragmentShader);

    glLinkProgram(id_);

    GLint correcto = GL_FALSE;
    glGetProgramiv(id_, GL_LINK_STATUS, &correcto);

    glDetachShader(id_, vertexShader);
    glDetachShader(id_, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (correcto != GL_TRUE)
    {
        std::cerr << "Error al enlazar el programa:\n";

        GLint longitud = 0;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &longitud);

        if (longitud > 0)
        {
            std::string log(
                static_cast<std::size_t>(longitud),
                '\0');

            glGetProgramInfoLog(
                id_,
                longitud,
                nullptr,
                log.data());

            std::cerr << log << '\n';
        }

        clear();

        return false;
    }

    return true;
}

void Shader::use() const
{
    glUseProgram(id_);
}

void Shader::clear()
{
    if (id_ != 0)
    {
        glDeleteProgram(id_);
        id_ = 0;
    }
}

int Shader::loc(const std::string &nombre) const
{
    const GLint ubicacion = glGetUniformLocation(
        id_,
        nombre.c_str());

    if (ubicacion == -1)
    {
        std::cerr
            << "Uniform no encontrado o no utilizado: "
            << nombre
            << '\n';
    }

    return ubicacion;
}

void Shader::set_uniform(
    const std::string &nombre,
    const glm::mat4 &matriz) const
{
    set_uniform(loc(nombre), matriz);
}

void Shader::set_uniform(
    int ubicacion,
    const glm::mat4 &matriz) const
{
    glProgramUniformMatrix4fv(
        id_,
        ubicacion,
        1,
        GL_FALSE,
        glm::value_ptr(matriz));
}

void Shader::set_uniform(
    const std::string &nombre,
    const glm::vec3 &vector) const
{
    set_uniform(loc(nombre), vector);
}

void Shader::set_uniform(
    int ubicacion,
    const glm::vec3 &vector) const
{
    glProgramUniform3fv(
        id_,
        ubicacion,
        1,
        glm::value_ptr(vector));
}