#include "GLUtil.h"
#include <fstream>

#define BITMAP_IMAGE_FILE_HEADER_SIZE 14
#define BITMAP_IMAGE_INFO_HEADER_SIZE 40

void ShaderCompileCheck(GLuint id)
{
	GLint result = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		GLchar error[256];
		glGetShaderInfoLog(id, sizeof(error), 0, error);
		MessageBox(NULL, error, "Shader Error", MB_OK);
	}
}

void ShaderLinkCheck(GLuint id)
{
	GLint  result = GL_FALSE;
	GLchar error[256];
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(id, sizeof(error), 0, error);
		MessageBox(NULL, error, "Program Error", MB_OK);
	}

	glValidateProgram(id);
	glGetProgramiv(id, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(id, sizeof(error), 0, error);
		MessageBox(NULL, error, "Program Error", MB_OK);
	}

	GLint attrCount, uniCount;
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &attrCount);
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniCount);

	std::ostringstream out;
	out.precision(6);
	out << "Attribute Count: " << attrCount << std::endl;
	out << "Uniform   Count: " << uniCount << std::endl;
	OutputDebugString(out.str().c_str());
}

GLuint LoadShaders(const char* vert, const char* frag)
{
	GLuint progID = glCreateProgram();
	GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertID, 1, &vert, 0);
	glCompileShader(vertID);
	ShaderCompileCheck(vertID);
	glAttachShader(progID, vertID);

	glShaderSource(fragID, 1, &frag, 0);
	glCompileShader(fragID);
	ShaderCompileCheck(fragID);
	glAttachShader(progID, fragID);

	glLinkProgram(progID);
	ShaderLinkCheck(progID);

	glDetachShader(progID, fragID);
	glDetachShader(progID, vertID);
	glDeleteShader(fragID);
	glDeleteShader(vertID);

	return progID;
}

GLuint LoadTexture(const char* filename)
{
	std::fstream hfile;
	hfile.open(filename, std::ios::in | std::ios::binary);

	if (!hfile.is_open())
		throw std::exception("Error: File not found");

	unsigned char fileHeader[BITMAP_IMAGE_FILE_HEADER_SIZE];
	unsigned char infoHeader[BITMAP_IMAGE_INFO_HEADER_SIZE];

	hfile.read(reinterpret_cast<char*>(fileHeader), BITMAP_IMAGE_FILE_HEADER_SIZE);
	hfile.read(reinterpret_cast<char*>(infoHeader), BITMAP_IMAGE_INFO_HEADER_SIZE);

	if (fileHeader[0] != 'B' && fileHeader[1] != 'M')
		throw std::exception("Error: File is not BitmapImage");
	
	hfile.seekg(0, std::ios::end);

	unsigned int fileLength = hfile.tellg();
	unsigned int fileSize   = ExtractValue<unsigned int>(fileHeader, 2);

	if (fileSize != fileLength)
		throw std::exception("Error: Invalid BitmapImage");

	UINT width        = ExtractValue<unsigned int>(infoHeader, 4);
	UINT height       = ExtractValue<unsigned int>(infoHeader, 8);
	UINT bitsPerPixel = ExtractValue<unsigned short>(infoHeader, 14);
	UINT size         = ((width * bitsPerPixel + 31) / 32) * 4 * height;
	
	unsigned char* data = new unsigned char[size];
	unsigned int pixelsOffset = ExtractValue<unsigned int>(fileHeader, 10);
	hfile.seekg(pixelsOffset, std::ios::beg);
    hfile.read(reinterpret_cast<char*>(data), size);
    hfile.close();

	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (bitsPerPixel > 24)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

template<typename T>
T ExtractValue(unsigned char* data, unsigned int pos)
{
	unsigned int index = sizeof(T);
	T result = 0;

	while (index--)
	{
		result = result << 8;
		result = result | data[pos + index];
	}
	return result;
}