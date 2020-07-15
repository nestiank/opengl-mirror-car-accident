#version 330

in vec3 normal;
in vec3 lightDir;
in vec3 halfVector;
in vec2 TexCoord;
uniform sampler2D texture1;
out vec4 FragColor;

vec4 lightAmbient = vec4(1.0, 1.0, 1.0, 1.0);
vec4 lightDiffuse = vec4(0.3, 0.3, 0.3, 1.0);
vec4 lightSpecular = vec4(1.0, 1.0, 1.0, 1.0);
vec4 matDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
vec4 matSpecular = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	vec4 color = lightAmbient * texture(texture1, TexCoord);
	vec3 n, h;
	float NdotL, NdotH;
	n = normalize(normal);
	NdotL = max(dot(n, lightDir), 0.0);
	color += matDiffuse * lightDiffuse * NdotL;
	h = normalize(halfVector);
	NdotH = max(dot(n, h), 0.0);
	color += matSpecular * lightSpecular * pow(NdotH, 10);

	FragColor = color;
}