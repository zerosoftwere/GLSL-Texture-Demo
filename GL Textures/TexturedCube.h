#pragma once

#include "GLUtil.h"

using namespace glm;

class TexturedCube
{
public:
	TexturedCube();
	~TexturedCube();

	void Update(float dt);
	void Draw(const mat4& view, const mat4& proj);

protected:
	GLuint mVBO;
	GLuint mProgID;
	GLuint mTextureID;

	GLint mMVPID;
	GLint mMITID;
	GLint mDirLightID;

	GLfloat   mRotation;
	vec3      mDirLight;

	static const char* vert;
	static const char* frag;
};

