#pragma once

namespace gecko {
	struct vec2
	{
		vec2() : x(0.0f), y(0.0f) { }
		vec2(float x, float y) : x(x), y(y) { }
		
		float x, y;
	};

	struct vec3
	{
	public:
		vec3() : x(0.0f), y(0.0f), z(0.0f) { }
		vec3(float x, float y, float z) : x(x), y(y), z(z) { }

		float x, y, z;
	private:
		float padding;
	};

	struct vec4
	{
		public:
		vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
		vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

		float x, y, z, w;
	};
};