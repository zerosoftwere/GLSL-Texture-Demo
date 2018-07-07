#include "TexturedCube.h"
#include "Vertex.h"
#include "BitmapImage.h"

const char* TexturedCube::vert = GLSL(130,
	uniform mat4 modelViewProjection;
	uniform mat4 modelInverseTranspose;

	attribute vec3 iPosition;
	attribute vec3 iNormal;
	attribute vec2 iTexcoord;

	out vec2  texcoord;
	out vec3  normal;

	void main() {
		vec3 normalW = (modelInverseTranspose * vec4(iNormal, 1.0f)).xyz;
		normal = normalize(normalW);

		texcoord  = iTexcoord;

		gl_Position = modelViewProjection * vec4(iPosition, 1.0f);
	}
);

const char* TexturedCube::frag = GLSL(130,
	uniform sampler2D texture;
	uniform vec3 dirLight;

	in vec2  texcoord;
	in vec3  normal;

	const float ambiant = 0.2f;

	void main() {
		float intensity = max(dot(dirLight, normalize(normal)), 0.0f);
		gl_FragColor = texture2D(texture, texcoord) * (ambiant + intensity);
	}
);

TexturedCube::TexturedCube(void)
	: mRotation(0.0f), mDirLight()
{
	mDirLight = normalize(vec3(1.0f, 1.0f, 1.0f));

	/** Cube Vertices **/
	vec3 cubePos[8] = {
		vec3( 1.0f, -1.0f,  1.0f),
		vec3( 1.0f,  1.0f,  1.0f),
		vec3(-1.0f,  1.0f,  1.0f),
		vec3(-1.0f, -1.0f,  1.0f),

		vec3( 1.0f, -1.0f, -1.0f),
		vec3( 1.0f,  1.0f, -1.0f),
		vec3(-1.0f,  1.0f, -1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
	};

	vec2 cubeTex[8] = {
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		
		vec2(0.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f),
	};

	vec3 cubeNor[8];
	for (size_t i = 0; i < 8; i++)
		cubeNor[i] = glm::normalize(cubePos[i]);

	Vertex cubeData[8];
	for (size_t i = 0; i < 8; i++)
		cubeData[i] = Vertex(cubePos[i], cubeNor[i], cubeTex[i]);

	const GLubyte cubeElem[] = {
		0, 1, 2, 3, // Front
		0, 4, 5, 1, // Right
		1, 5, 6, 2, // Top
		2, 6, 7, 3, // Left
		4, 7, 6, 5, // Back
		0, 3, 7, 4  // Bottom
	};
	
	mProgID = LoadShaders(TexturedCube::vert, TexturedCube::frag);

	mMVPID    = glGetUniformLocation(mProgID, "modelViewProjection");
	mMITID     = glGetUniformLocation(mProgID, "modelInverseTranspose");
	mDirLightID = glGetUniformLocation(mProgID, "dirLight");

	GLint posID = glGetAttribLocation(mProgID, "iPosition");
	GLint norID = glGetAttribLocation(mProgID, "iNormal");
	GLint texID = glGetAttribLocation(mProgID, "iTexcoord");

	/** Create and bind Vertex Array **/
	glGenVertexArrays(1, &mVBO);
	glBindVertexArray(mVBO);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);

	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeElem), cubeElem, GL_STATIC_DRAW);
	
	/** Define Vertex attributes **/
	glEnableVertexAttribArray(posID);
	glEnableVertexAttribArray(norID);
	glEnableVertexAttribArray(texID);

	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(norID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
	glVertexAttribPointer(texID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 2));
	
	/** Unbind Vertex array **/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(posID);

	/** Generate and load Texture **/
	mTextureID = LoadTexture("bricks.bmp");
}


TexturedCube::~TexturedCube(void)
{
	glDeleteProgram(mProgID);
	glDeleteVertexArrays(1, &mVBO);
}

void TexturedCube::Update(float dt)
{
	mRotation += 2 * dt;
}

void TexturedCube::Draw(const mat4& view, const mat4& proj)
{
	glUseProgram(mProgID);
	glBindVertexArray(mVBO);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	
	glm::mat4 model = glm::rotate(glm::mat4(), mRotation, vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 modelViewProjection = proj * view * model;

	glm::mat4 modelInverseTranspose = glm::inverse(model);
	modelInverseTranspose = glm::transpose(modelInverseTranspose);

	glUniformMatrix4fv(mMVPID, 1, GL_FALSE, glm::value_ptr(modelViewProjection));
	glUniformMatrix4fv(mMITID, 1, GL_FALSE, glm::value_ptr(modelInverseTranspose));
	glUniform3fv(mDirLightID, 1, glm::value_ptr(mDirLight));

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, 0);
}
