//Editted by Lim, HyunSang, Korea Univ
#version 330

layout(location = 0)in vec3 aPos;
layout(location = 1)in vec3 aColor;
out vec4 Color;
uniform mat4 modelview;
uniform mat4 projection;

void main()
{
	gl_Position = projection * modelview * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	Color = vec4(aColor, 1.0f);
}