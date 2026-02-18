#version 430 core
layout(location = 0) in vec2 aPos;

uniform float offset;

void main(void)
{ 
	gl_Position = vec4(aPos.x+offset, aPos.y, 0.0f, 1.0f);
}