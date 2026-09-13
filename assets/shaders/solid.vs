#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

// Matriz proporcionada en las filminas del practico 2.
// GLSL recibe las columnas de la matriz.
const mat3 kRotacionFija = mat3(
    vec3( 0.3686, -0.1454, -0.3119),
    vec3( 0.0000,  0.5438, -0.2536),
    vec3(-0.2581, -0.2077, -0.4454)
);

void main()
{
    vertexColor = aColor;

    gl_Position = vec4(
        kRotacionFija * aPos,
        1.0
    );
}