#pragma once
#include <glm/glm.hpp>

using namespace glm;

struct VertexPos
{
	VertexPos()
		: pos() { }
	VertexPos(float x, float y, float z)
		: pos(x, y, z) { }
	VertexPos(const vec3& pos) 
		: pos(pos) { }

	vec3 pos;
};

struct VertexNor : public VertexPos
{
	VertexNor()
		: VertexPos(), nor() { }
	VertexNor(float x, float y, float z, float nx, float ny, float nz)
		: VertexPos(x, y, z), nor(nx, ny, nz) { }
	VertexNor(const vec3& pos, const vec3& nor)
		: VertexPos(pos), nor(nor) { }

	vec3 nor;
};

struct VertexCol : public VertexPos
{
	VertexCol()
		: VertexPos(), col(1.0f, 0.0f, 0.0f, 1.0f) { }
	VertexCol(float x, float y, float z, float r, float g, float b, float a) 
		: VertexPos(x, y, z), col(r, g, b, a) { }
	VertexCol(const vec3& pos, const vec4& col) : VertexPos(pos), col(col) { }

	vec4 col;
};

struct VertexTex : public VertexPos
{
	VertexTex()
		: VertexPos(), tex() { }
	VertexTex(float x, float y, float z, float u, float v)
		: VertexPos(x, y, z), tex(u, v) { }
	VertexTex(const vec3& pos, const vec2& tex)
		: VertexPos(pos), tex(tex) { }

	vec2 tex;
};

struct Vertex
{
	Vertex()
		: pos(), nor(), tex()
	{ }

	Vertex(const vec3& pos, const vec3& nor, const vec2& tex)
		: pos(pos), nor(nor), tex(tex)
	{ }

	vec3 pos;
	vec3 nor;
	vec2 tex;
};