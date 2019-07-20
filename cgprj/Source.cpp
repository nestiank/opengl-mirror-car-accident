//Editted by Lim, HyunSang, Korea Univ
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <glew.h>
#include <glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace std;

#define PI 3.141592

// shader variables
GLuint program, vertShader, fragShader = 0;
GLuint tprogram, tvertShader, tfragShader = 0;
GLuint skyprogram, skyvertShader, skyfragShader = 0;
unsigned int VAO, VBO, NBO, TBO, TEXTURE;
unsigned int VAO2, VBO2, NBO2, TBO2, TEXTURE2;
unsigned int skyVAO, skyVBO, skyIBO, skyTEXTURE;
float WIDTH = 600;
float HEIGTH = 600;
float *vertex, *texturecoord, *normal;
float *wheelvertex, *wheeltexturecoord, *wheelnormal;
unsigned int N, N2;

// car parameters
int timeDelay = 10;
GLfloat wheelAngle = 0.0f, wheelAngularVelocity = PI / 15.0f, theta = 0.0f, theta2 = 0.0f;
GLfloat position = -1.2f, position2 = 1.2f, velocity = PI / 300.0f;
GLfloat acceleration = PI / 30000.0f, wheelacceleration = PI / 1500.0f;

bool collide = false;

void ReadOBJ();
	void ReadChassisOBJ();
	void ReadwheelOBJ();
vector<string> tokenize(const string& data, const char delimiter);
void CreateProgram();
void TextureCreateProgram();
void SkyCreateProgram();
unsigned int createShader(const char* src, GLenum type);
void initTexture();
	void initChassisTexture();
	void initWheelTexture();
	void initSkyTexture();
void DataTransfer();
void SkyDataTransfer();

// OBJ
void ReadOBJ()
{
	ReadChassisOBJ();
	ReadwheelOBJ();
}
void ReadChassisOBJ()
{
	vector<float> v;
	vector<float> t;
	vector<float> n;
	vector<unsigned int> vindex;
	vector<unsigned int> tindex;
	vector<unsigned int> nindex;

	string OBJCode;
	ifstream OBJFile("car.obj");

	while (!OBJFile.eof()) {
		string s;
		getline(OBJFile, s);
		if (s.size() != 0) {
			vector<string> temp = tokenize(s, ' ');
			if (temp[0] == "v") {
				for (int k = 1; k < temp.size(); k++) {
					if (temp[k].size() != 0) v.push_back(atof(temp[k].c_str()));
				}
			}
			else if (temp[0] == "vn") {
				for (int k = 1; k < temp.size(); k++) {
					if (temp[k].size() != 0) n.push_back(atof(temp[k].c_str()));
				}
			}
			else if (temp[0] == "vt") {
				for (int k = 1; k < temp.size(); k++) {
					if (temp[k].size() != 0) {
						t.push_back(atof(temp[k].c_str()));
					}
				}
			}
			else if (temp[0] == "f") {
				vector<string> temp1 = tokenize(temp[1], '/');
				vector<string> temp2 = tokenize(temp[2], '/');
				vector<string> temp3 = tokenize(temp[3], '/');

				vindex.push_back(atoi(temp1[0].c_str()) - 1);
				vindex.push_back(atoi(temp2[0].c_str()) - 1);
				vindex.push_back(atoi(temp3[0].c_str()) - 1);

				tindex.push_back(atoi(temp1[1].c_str()) - 1);
				tindex.push_back(atoi(temp2[1].c_str()) - 1);
				tindex.push_back(atoi(temp3[1].c_str()) - 1);

				nindex.push_back(atoi(temp1[2].c_str()) - 1);
				nindex.push_back(atoi(temp2[2].c_str()) - 1);
				nindex.push_back(atoi(temp3[2].c_str()) - 1);
			}
		}
	}
	N = vindex.size();
	vertex = new float[3 * N];
	texturecoord = new float[3 * N];
	normal = new float[3 * N];

	for (int i = 0; i < N / 3; i++) {
		for (int j = 0; j < 3; j++) {
			vertex[3 * (3 * i + j) + 0] = v[3 * vindex[3 * i + j] + 0];
			vertex[3 * (3 * i + j) + 1] = v[3 * vindex[3 * i + j] + 1];
			vertex[3 * (3 * i + j) + 2] = v[3 * vindex[3 * i + j] + 2];

			if (t.size()) {
				texturecoord[3 * (3 * i + j) + 0] = t[3 * tindex[3 * i + j] + 0];
				texturecoord[3 * (3 * i + j) + 1] = t[3 * tindex[3 * i + j] + 1];
				texturecoord[3 * (3 * i + j) + 2] = t[3 * tindex[3 * i + j] + 2];
			}
			normal[3 * (3 * i + j) + 0] = n[3 * nindex[3 * i + j] + 0];
			normal[3 * (3 * i + j) + 1] = n[3 * nindex[3 * i + j] + 1];
			normal[3 * (3 * i + j) + 2] = n[3 * nindex[3 * i + j] + 2];
		}
	}
}
void ReadwheelOBJ()
{
	vector<float> v;
	vector<float> t;
	vector<float> n;
	vector<unsigned int> vindex;
	vector<unsigned int> tindex;
	vector<unsigned int> nindex;

	string OBJCode;
	ifstream OBJFile("wheel.obj");

	while (!OBJFile.eof()) {
		string s;
		getline(OBJFile, s);
		if (s.size() != 0) {
			vector<string> temp = tokenize(s, ' ');
			if (temp[0] == "v") {
				for (int k = 1; k < temp.size(); k++) {
					if (temp[k].size() != 0) v.push_back(atof(temp[k].c_str()));
				}
			}
			else if (temp[0] == "vn") {
				for (int k = 1; k < temp.size(); k++) {
					if (temp[k].size() != 0) n.push_back(atof(temp[k].c_str()));
				}
			}
			else if (temp[0] == "vt") {
				for (int k = 1; k < temp.size(); k++) {
					if (temp[k].size() != 0) {
						t.push_back(atof(temp[k].c_str()));
					}
				}
			}
			else if (temp[0] == "f") {
				vector<string> temp1 = tokenize(temp[1], '/');
				vector<string> temp2 = tokenize(temp[2], '/');
				vector<string> temp3 = tokenize(temp[3], '/');

				vindex.push_back(atoi(temp1[0].c_str()) - 1);
				vindex.push_back(atoi(temp2[0].c_str()) - 1);
				vindex.push_back(atoi(temp3[0].c_str()) - 1);

				tindex.push_back(atoi(temp1[1].c_str()) - 1);
				tindex.push_back(atoi(temp2[1].c_str()) - 1);
				tindex.push_back(atoi(temp3[1].c_str()) - 1);

				nindex.push_back(atoi(temp1[2].c_str()) - 1);
				nindex.push_back(atoi(temp2[2].c_str()) - 1);
				nindex.push_back(atoi(temp3[2].c_str()) - 1);
			}
		}
	}
	N2 = vindex.size();
	wheelvertex = new float[3 * N2];
	wheeltexturecoord = new float[3 * N2];
	wheelnormal = new float[3 * N2];

	for (int i = 0; i < N2 / 3; i++) {
		for (int j = 0; j < 3; j++) {
			wheelvertex[3 * (3 * i + j) + 0] = v[3 * vindex[3 * i + j] + 0];
			wheelvertex[3 * (3 * i + j) + 1] = v[3 * vindex[3 * i + j] + 1];
			wheelvertex[3 * (3 * i + j) + 2] = v[3 * vindex[3 * i + j] + 2];

			if (t.size()) {
				wheeltexturecoord[3 * (3 * i + j) + 0] = t[3 * tindex[3 * i + j] + 0];
				wheeltexturecoord[3 * (3 * i + j) + 1] = t[3 * tindex[3 * i + j] + 1];
				wheeltexturecoord[3 * (3 * i + j) + 2] = t[3 * tindex[3 * i + j] + 2];
			}
			wheelnormal[3 * (3 * i + j) + 0] = n[3 * nindex[3 * i + j] + 0];
			wheelnormal[3 * (3 * i + j) + 1] = n[3 * nindex[3 * i + j] + 1];
			wheelnormal[3 * (3 * i + j) + 2] = n[3 * nindex[3 * i + j] + 2];
		}
	}
}
vector<string> tokenize(const string & data, const char delimiter)
{
	vector<string> result;
	string token;
	stringstream ss(data);

	while (getline(ss, token, delimiter)) {
		result.push_back(token);
	}
	return result;
}

// shader
void Initialize(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(WIDTH, HEIGTH);
	glutCreateWindow("hw03");
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH);

	GLenum errorCode = glewInit();
	if (GLEW_OK != errorCode) {
		cout << "Error: GLEW Initialization is failed\n" << glewGetErrorString(errorCode) << endl;
		exit(EXIT_FAILURE);
	}
	CreateProgram();
	TextureCreateProgram();
	SkyCreateProgram();
	initTexture();
	DataTransfer();
	SkyDataTransfer();
}
void CreateProgram()
{
	// car drawing shader
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	try {
		vShaderFile.open("Vertex.glsl");
		fShaderFile.open("Fragment.glsl");
		stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		stringstream fShaderStream;
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	vertShader = createShader(vShaderCode, GL_VERTEX_SHADER);
	fragShader = createShader(fShaderCode, GL_FRAGMENT_SHADER);
	GLuint p = glCreateProgram();
	glAttachShader(p, vertShader);
	glAttachShader(p, fragShader);
	glLinkProgram(p);

	int success;
	char infoLog[512];
	glGetProgramiv(p, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(p, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s\n", infoLog);
	}
	program = p;
}
void TextureCreateProgram()
{
	// car drawing shader
	string tvertexCode;
	string tfragmentCode;
	ifstream tvShaderFile;
	ifstream tfShaderFile;
	try {
		tvShaderFile.open("textureVertex.glsl");
		tfShaderFile.open("textureFragment.glsl");
		stringstream tvShaderStream;
		tvShaderStream << tvShaderFile.rdbuf();
		stringstream tfShaderStream;
		tfShaderStream << tfShaderFile.rdbuf();
		tvShaderFile.close();
		tfShaderFile.close();
		tvertexCode = tvShaderStream.str();
		tfragmentCode = tfShaderStream.str();
	}
	catch (ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* tvShaderCode = tvertexCode.c_str();
	const char* tfShaderCode = tfragmentCode.c_str();

	tvertShader = createShader(tvShaderCode, GL_VERTEX_SHADER);
	tfragShader = createShader(tfShaderCode, GL_FRAGMENT_SHADER);
	GLuint p = glCreateProgram();
	glAttachShader(p, tvertShader);
	glAttachShader(p, tfragShader);
	glLinkProgram(p);

	int success;
	char infoLog[512];
	glGetProgramiv(p, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(p, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s\n", infoLog);
	}
	tprogram = p;
}
void SkyCreateProgram()
{
	// car drawing shader
	string skyvertexCode;
	string skyfragmentCode;
	ifstream skyvShaderFile;
	ifstream skyfShaderFile;
	try {
		skyvShaderFile.open("skyVertex.glsl");
		skyfShaderFile.open("skyFragment.glsl");
		stringstream skyvShaderStream;
		skyvShaderStream << skyvShaderFile.rdbuf();
		stringstream skyfShaderStream;
		skyfShaderStream << skyfShaderFile.rdbuf();
		skyvShaderFile.close();
		skyfShaderFile.close();
		skyvertexCode = skyvShaderStream.str();
		skyfragmentCode = skyfShaderStream.str();
	}
	catch (ifstream::failure e) {
		std::cout << "ERROR::SKY_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* skyvShaderCode = skyvertexCode.c_str();
	const char* skyfShaderCode = skyfragmentCode.c_str();

	skyvertShader = createShader(skyvShaderCode, GL_VERTEX_SHADER);
	skyfragShader = createShader(skyfShaderCode, GL_FRAGMENT_SHADER);
	GLuint p = glCreateProgram();
	glAttachShader(p, skyvertShader);
	glAttachShader(p, skyfragShader);
	glLinkProgram(p);

	int success;
	char infoLog[512];
	glGetProgramiv(p, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(p, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s\n", infoLog);
	}
	skyprogram = p;
}
unsigned int createShader(const char* src, GLenum type)
{
	unsigned int shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		if (type == 35633) {
			printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", infoLog);
		}
		else {
			printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", infoLog);
		}
	}

	return shader;
}

// texture
void initTexture()
{
	initChassisTexture();
	initWheelTexture();
	initSkyTexture();
}
void initChassisTexture()
{
	glGenTextures(1, &TEXTURE);
	glBindTexture(GL_TEXTURE_2D, TEXTURE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/signature.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
void initWheelTexture()
{
	glGenTextures(1, &TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TEXTURE2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/wheel.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
void initSkyTexture()
{
	glGenTextures(1, &skyTEXTURE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyTEXTURE);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/cubemap1.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	data = stbi_load("textures/cubemap2.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	data = stbi_load("textures/cubemap3.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	data = stbi_load("textures/cubemap4.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	data = stbi_load("textures/cubemap5.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	data = stbi_load("textures/cubemap6.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// GPU upload
void DataTransfer()
{
	// Generating Buffer Objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &TBO);
	// Transferring Vertex data to Device
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * N, vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * N, normal, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * N, texturecoord, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	// Generating Buffer Objects
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &NBO2);
	glGenBuffers(1, &TBO2);
	// Transferring Vertex data to Device
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * N2, wheelvertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, NBO2);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * N2, wheelnormal, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, TBO2);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * N2, wheeltexturecoord, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void SkyDataTransfer()
{
	float skyvertex[8 * 3] = {
		 1.0f, -1.0f,  1.0f,//Vertex0
		 1.0f, -1.0f, -1.0f,//Vertex1
		-1.0f, -1.0f, -1.0f,//Vertex2
		-1.0f, -1.0f,  1.0f,//Vertex3
		 1.0f,  1.0f,  1.0f,//Vertex4
		 1.0f,  1.0f, -1.0f,//Vertex5
		-1.0f,  1.0f, -1.0f,//Vertex6
		-1.0f,  1.0f,  1.0f //Vertex7
	};
	unsigned int index[6 * 6] = {
		0, 2, 1, 0, 3, 2,//Face1
		4, 3, 0, 4, 7, 3,//Face2
		5, 0, 1, 5, 4, 0,//Face3
		5, 7, 4, 5, 6, 7,//Face4
		1, 2, 6, 1, 6, 5,//Face5
		7, 2, 3, 7, 6, 2//Face6		
	};

	//Generating Buffer Objects
	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);
	glGenBuffers(1, &skyIBO);
	//Transferring Vertex data to Device
	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyvertex), skyvertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Tranferring Index data to Device
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// skybox
	glDepthFunc(GL_LEQUAL);
	glUseProgram(skyprogram);
	glBindVertexArray(skyVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyTEXTURE);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

	// chassis 1
	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, TEXTURE);

	// y-rotation 20 degrees, z-rotation -10 degrees
	float MVMat[16] = { 0.93, 0.16, 0.34, 0, -0.17, 0.98, 0, 0, -0.34, 0.06, 0.94, 0, position * cos(20 * PI / 180), position * sin(20 * PI / 180), -0.3, 2 };
	float PMat[16] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };

	glBindVertexArray(VAO);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMat);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, PMat);
	glDrawArrays(GL_TRIANGLES, 0, N);

	// chassis 2
	float MVMatB[16] = { 0.93, 0.16, 0.34, 0, -0.17, 0.98, 0, 0, -0.34, 0.06, 0.94, 0, position2 * cos(20 * PI / 180), position2 * sin(20 * PI / 180) - 0.3, 0, 2 };
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMatB);
	glDrawArrays(GL_TRIANGLES, 0, N);

	// wheels 1
	glUseProgram(tprogram);
	glBindTexture(GL_TEXTURE_2D, TEXTURE2);

	float MVMat2[16] = { cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1, 0, position * cos(20 * PI / 180) + 0.5, position * sin(20 * PI / 180) - 0.2, -0.25 - 0.3, 2 };
	float MVMat3[16] = { cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1, 0, position * cos(20 * PI / 180) + 0.4, position * sin(20 * PI / 180) - 0.22, 0.75 - 0.3, 2 };
	float MVMat4[16] = { cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1, 0, position * cos(20 * PI / 180) - 0.2, position * sin(20 * PI / 180) - 0.33, -0.75 - 0.3, 2 };
	float MVMat5[16] = { cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1, 0, position * cos(20 * PI / 180) - 0.3, position * sin(20 * PI / 180) - 0.35, 0.2-0.35 - 0.3, 2 };

	glBindVertexArray(VAO2);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMat2);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, PMat);
	glDrawArrays(GL_TRIANGLES, 0, N2);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMat3);
	glDrawArrays(GL_TRIANGLES, 0, N2);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMat4);
	glDrawArrays(GL_TRIANGLES, 0, N2);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMat5);
	glDrawArrays(GL_TRIANGLES, 0, N2);

	// wheels 2
	glBindTexture(GL_TEXTURE_2D, TEXTURE2);

	float MVMat6[16] = { cos(theta2), -sin(theta2), 0, 0, sin(theta2), cos(theta2), 0, 0, 0, 0, 1, 0, position2 * cos(20 * PI / 180) + 0.5, position2 * sin(20 * PI / 180) - 0.2 - 0.3, -0.25, 2 };
	float MVMat7[16] = { cos(theta2), -sin(theta2), 0, 0, sin(theta2), cos(theta2), 0, 0, 0, 0, 1, 0, position2 * cos(20 * PI / 180) + 0.4, position2 * sin(20 * PI / 180) - 0.22 - 0.3, 0.75, 2 };
	float MVMat8[16] = { cos(theta2), -sin(theta2), 0, 0, sin(theta2), cos(theta2), 0, 0, 0, 0, 1, 0, position2 * cos(20 * PI / 180) - 0.2, position2 * sin(20 * PI / 180) - 0.33 - 0.3, -0.75, 2 };
	float MVMat9[16] = { cos(theta2), -sin(theta2), 0, 0, sin(theta2), cos(theta2), 0, 0, 0, 0, 1, 0, position2 * cos(20 * PI / 180) - 0.3, position2 * sin(20 * PI / 180) - 0.35 - 0.3, 0.25, 2 };
	
	glBindVertexArray(VAO2);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMat6);
	glDrawArrays(GL_TRIANGLES, 0, N2);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMat7);
	glDrawArrays(GL_TRIANGLES, 0, N2);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMat8);
	glDrawArrays(GL_TRIANGLES, 0, N2);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, MVMat9);
	glDrawArrays(GL_TRIANGLES, 0, N2);

	glBindVertexArray(0);
	glUseProgram(0);
	glutSwapBuffers();
}

// collision detection
bool collision() {
	if (position2 - position < 1.5 + 0.0001) collide = true;
	return collide;
}

// basics
void Reshape(int w, int h)
{
	WIDTH = (w == 0) ? 1 : w;
	HEIGTH = (h == 0) ? 1 : h;
	glViewport(0, 0, WIDTH, HEIGTH);
}
void timerFunc(int value)
{
	if (velocity < 0.0001) {
		velocity = 0.0f;
		wheelAngularVelocity = 0.0f;
	}
	
	if (!collision()) {
		position += velocity;

		theta += wheelAngularVelocity;
		if (theta > 2 * PI) theta = 0;
	}
	else {
		position -= velocity / 2;
		position2 += velocity / 2;
		velocity -= acceleration;
		wheelAngularVelocity -= wheelacceleration;

		if (fabs(velocity) < 0.001) velocity = 0;
		if (fabs(wheelAngularVelocity) < 0.02) wheelAngularVelocity = 0;

		theta -= wheelAngularVelocity;
		if (theta < -2 * PI) theta = 0;
		theta2 += wheelAngularVelocity;
		if (theta2 > 2 * PI) theta2 = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(timeDelay, timerFunc, 1);
}
int main(int argc, char** argv)
{
	ReadOBJ();
	Initialize(argc, argv);
	glutDisplayFunc(Display);
	glutTimerFunc(timeDelay, timerFunc, 1);
	glutMainLoop();
	return 0;
}