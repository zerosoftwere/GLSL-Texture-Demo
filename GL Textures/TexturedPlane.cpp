#include "TexturedPlane.h"
#include "Vertex.h"
#include "BitmapImage.h"

const char* TexturedPlane::vert = GLSL(130,
	uniform mat4 MVP;

	attribute vec3 iPosition;
	attribute vec2 iTexcoord;
	
	varying vec2 texcoord;

	void main()
	{
		gl_Position  = MVP * vec4(iPosition, 1.0f);
		texcoord = iTexcoord;
	}
);
const char* TexturedPlane::frag = GLSL(130,
	uniform sampler2D texture;

	varying vec2 texcoord;
	
	void main()
	{
		gl_FragColor = texture2D(texture, texcoord);
	}
);

TexturedPlane::TexturedPlane()
{
	const VertexTex planeData[] = {
		VertexTex( 1.0f, -1.0f, 0.0f, 1.0f, 0.0f),
		VertexTex( 1.0f,  1.0f, 0.0f, 1.0f, 1.0f),
		VertexTex(-1.0f,  1.0f, 0.0f, 0.0f, 1.0f),
		VertexTex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f)
	};

	mProgramID = LoadShaders(TexturedPlane::vert, TexturedPlane::frag);
	
	mTextureLoc = glGetUniformLocation(mProgramID, "texture");
	mMVPLoc     = glGetUniformLocation(mProgramID, "MVP");
	
	GLint positionLoc, texcoordLoc;
	positionLoc = glGetAttribLocation(mProgramID, "iPosition");
	texcoordLoc = glGetAttribLocation(mProgramID, "iTexcoord");
	
	glGenVertexArrays(1, &mVertexID);
	glBindVertexArray(mVertexID);

	glGenBuffers(1, &mBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeData), planeData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(texcoordLoc);

	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTex), (void*) 0);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTex), (void*) sizeof(glm::vec3));

	glBindVertexArray(0);

	BitmapImage img("bricks.bmp");

	std::stringstream out;
	out.precision(6);
	out << "Image Res   : " << img.Width() << "x" << img.Height() << std::endl;
	out << "Image Depth : " << img.BitsPerPixel() << std::endl;
	OutputDebugString(out.str().c_str());

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.Width(), img.Height(), 0, GL_BGR, GL_UNSIGNED_BYTE, img.Data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}


TexturedPlane::~TexturedPlane(void)
{
	glDeleteVertexArrays(1, &mVertexID);
	glDeleteBuffers(1, &mBufferID);
	glDeleteProgram(mProgramID);
	glDeleteTextures(1, &mTextureID);
}

void TexturedPlane::Draw(const glm::mat4& view, const glm::mat4& proj)
{
	glUseProgram(mProgramID);
	glBindVertexArray(mVertexID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glm::mat4 MVP = proj * view;
	glUniformMatrix4fv(mMVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	glDrawArrays(GL_QUADS, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}