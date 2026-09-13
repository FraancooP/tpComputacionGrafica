#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 uModel;
uniform mat4 uAjuste;

void main()
{
    vertexColor = aColor;

    gl_Position =
        uAjuste * uModel * vec4(aPos, 1.0);
}