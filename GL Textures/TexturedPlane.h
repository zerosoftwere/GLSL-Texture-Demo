#pragma once

#include "GLUtil.h"

class TexturedPlane
{
public:
	TexturedPlane();
	~TexturedPlane();

	void Draw(const glm::mat4& view, const glm::mat4& proj);

protected:
	GLuint mProgramID;
	GLuint mVertexID;
	GLuint mBufferID;
	GLuint mTextureID;

	GLint mTextureLoc;
	GLint mMVPLoc;


	static const char* vert;
	static const char* frag;
};

