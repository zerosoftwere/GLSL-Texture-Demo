#include "GLApp.h"

#include "GLUtil.h"
#include "TexturedCube.h"

#if defined (DEBUG) | defined (_DEBUG)
#include <crtdbg.h>
#endif

#define WIDTH  640
#define HEIGHT 480

class GLDemo : public GLApp
{

public:
	GLDemo(int width, int height, std::string caption);
	~GLDemo();

	bool DeviceCheck() override;
	void UpdateScene(float dt) override;
	void DrawScene() override;

private:
	glm::mat4 mProj;
	glm::mat4 mView;

	TexturedCube* mCube;
};

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, INT nCmdShow)
{
#if defined (DEBUG) | defined (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	GLDemo app(WIDTH, HEIGHT, "GLSL Cube Demo");
	if (!app.DeviceCheck())
	{
		MessageBox(NULL, "OpenGL driver version 3.0 or later is required", "Device Driver Not Suppported", MB_OK);
		exit(EXIT_FAILURE);
	}
	app.Run();

	return 0;
}

GLDemo::GLDemo(int width, int height, std::string caption)
	: GLApp(width, height, caption)
{
	/** Initialize View and Projection**/
	mView = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mProj = glm::perspective(PI * 0.25f, AspectRatio(), 0.1f, -100.0f);

	mCube = new TexturedCube();
}

GLDemo::~GLDemo()
{
	delete mCube;
}

void GLDemo::UpdateScene(float dt)
{
	mCube->Update(dt);
}

void GLDemo::DrawScene()
{
	glClearColor(0.2f, 0.2f, 0.23f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	mCube->Draw(mView, mProj);

	glBindVertexArray(0);
	glUseProgram(0);
}

bool GLDemo::DeviceCheck()
{
	GLint version;
	glGetIntegerv(GL_MAJOR_VERSION, &version);

	if (version < 3) return false;
	else return true;
}
