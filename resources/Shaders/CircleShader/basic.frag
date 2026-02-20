#version 430 core

out vec4 outColor;

uniform vec4 u_Color;

void main(void)
{
	outColor = u_Color;
}