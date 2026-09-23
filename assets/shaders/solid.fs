#version 450 core

in vec3 vNormal;

uniform vec3 uColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}
