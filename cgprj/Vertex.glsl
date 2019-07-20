#version 330

layout(location = 0)in vec3 Pos;
layout(location = 1)in vec3 Norm;
layout(location = 2)in vec2 tCoord;

float PI = 3.141592653589793238;
out vec3 lightDir;
out vec3 halfVector;
out vec2 TexCoord;

out vec3 Normal;
out vec3 Position;

vec3 cameraPos = vec3(0.0, 0.0, 1.0);
vec3 lightPos = vec3(0.0, 3.0, 3.0);

uniform mat4 modelview;
uniform mat4 projection;

void main()
{
	vec4 position = modelview * vec4(Pos.xyz, 1.0);
	vec3 view = normalize(cameraPos - position.xyz);
	lightDir = normalize(lightPos - position.xyz);
	halfVector = normalize(view + lightDir);
	TexCoord = vec2(tCoord.y, -tCoord.x);
	gl_Position = projection * modelview * vec4(Pos.xyz, 1.0);

	Normal = mat3(modelview) * Norm;
	Position = vec3(modelview * vec4(Pos, 1.0));
}