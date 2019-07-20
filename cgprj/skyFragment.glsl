#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
	FragColor = 0.5 * (smoothstep(0.16, 0.34, texture(skybox, TexCoords)) + smoothstep(0.66, 0.84, texture(skybox, TexCoords)));
}