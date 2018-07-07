#pragma once

#include "GLUtil.h"

using namespace glm;

class ColoredCube
{
public:
	ColoredCube();
	~ColoredCube();

	void Update(float dt);
	void Draw(const mat4& view, const mat4& proj);

protected:
	GLuint mVBO;
	GLuint mProgID;

	GLint mModelID;
	GLint mViewID;
	GLint mProjID;

	GLfloat   mRotation;
};

