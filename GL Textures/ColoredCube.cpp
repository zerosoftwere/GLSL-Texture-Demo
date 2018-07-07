#include "ColoredCube.h"
#include "Vertex.h"

const char* vert = GLSL(130,
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 proj;

	attribute vec3 iPosition;
	attribute vec4 iColor;
	varying   vec4 dColor;

	void main() {
		dColor = iColor;
		gl_Position = proj * view * model * vec4(iPosition, 1.0f);
	}
);

const char* frag = GLSL(130,
	varying vec4 dColor;
	void main() {
		gl_FragColor = dColor;
	}
);

ColoredCube::ColoredCube(void)
	: mRotation(0.0f)
{
	/** Cube Vertices **/
	const VertexCol cubeData[] = {
		VertexCol( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		VertexCol( 1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		VertexCol(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		VertexCol(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f),

		VertexCol( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		VertexCol( 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		VertexCol(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		VertexCol(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
	};

	const GLubyte cubeElem[] = {
		0, 1, 2, 3, // Front
		0, 4, 5, 1, // Right
		1, 5, 6, 2, // Top
		2, 6, 7, 3, // Left
		4, 7, 6, 5, // Back
		0, 3, 7, 4  // Bottom
	};
	
	mProgID = LoadShaders(vert, frag);

	mModelID = glGetUniformLocation(mProgID, "model");
	mViewID  = glGetUniformLocation(mProgID, "view");
	mProjID  = glGetUniformLocation(mProgID, "proj");

	GLint posID = glGetAttribLocation(mProgID, "iPosition");
	GLint colID = glGetAttribLocation(mProgID, "iColor");

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
	glEnableVertexAttribArray(colID);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexCol), 0);
	glVertexAttribPointer(colID, 4, GL_FLOAT, GL_FALSE, sizeof(VertexCol), (void*)sizeof(glm::vec3));
	
	/** Unbind Vertex array **/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(posID);
}


ColoredCube::~ColoredCube(void)
{
	glDeleteProgram(mProgID);
	glDeleteVertexArrays(1, &mVBO);
}

void ColoredCube::Update(float dt)
{
	mRotation += 2 * dt;
}

void ColoredCube::Draw(const mat4& view, const mat4& proj)
{
	glUseProgram(mProgID);
	glBindVertexArray(mVBO);
	
	glm::mat4 model = glm::rotate(glm::mat4(), mRotation, vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(mProjID, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(mViewID, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(mModelID, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, 0);
}
