#include "GLApp.h"

GLApp::GLApp(int width, int height, std::string caption)
	: mCaption(caption), mWidth(width), mHeight(height)
{
	if (!glfwInit())
	{
		MessageBox(NULL, "Failed to initialize glfw", "Deivce Error", MB_OK); 
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_RESIZABLE, false);
	mWindow = glfwCreateWindow(width, height, caption.c_str(), NULL, NULL);
	if (!mWindow)
	{
		MessageBox(NULL, "Failed to create window", "Deivce Error", MB_OK); 
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(mWindow);

	std::ostringstream out;
	out.precision(6);
	out << "Driver Version: " << glGetString(GL_VERSION) << std::endl;
	out << "GLSL   Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	OutputDebugString(out.str().c_str());

	glewExperimental = true;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		MessageBox(NULL, (char*) glewGetErrorString(glewError), "GLEW Error", MB_OK);
		exit(EXIT_FAILURE);
	}

	if (GLEW_ARB_vertex_array_object)
	{
		OutputDebugString("genVertexArray supported!\n");
	}

	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}


GLApp::~GLApp(void)
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void GLApp::Run()
{
	/***************Swap with Timer Class***************************/
	__int64 countPerSec, prevTimeStamp, curTimeStamp;

	QueryPerformanceFrequency((LARGE_INTEGER*) &countPerSec);
	float secPerCount = 1.0f / (float) countPerSec;
	QueryPerformanceCounter((LARGE_INTEGER*) &prevTimeStamp);
	/***************************************************************/

	while(!glfwWindowShouldClose(mWindow))
	{
		glViewport(0, 0, mWidth, mHeight);
		
		QueryPerformanceCounter((LARGE_INTEGER*) &curTimeStamp);
		float dt = (float) (curTimeStamp - prevTimeStamp) * secPerCount;

		UpdateScene(dt);
		DrawScene();

		CalculateFrameStats(dt);
		prevTimeStamp = curTimeStamp;

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

float GLApp::AspectRatio()
{
	return (float) mWidth / (float) mHeight;
}

void GLApp::CalculateFrameStats(float dt)
{
	static float elapsedTime = 0.0f;
	static int   frameCount  = 0;

	frameCount++;
	if (elapsedTime > 1.0f)
	{
		float fps = (float) frameCount;
		float mpf = 1000.0f / fps;

		std::ostringstream out;
		out.precision(6);
		out << mCaption << " FPS: " << fps << " Frame Time: " << mpf << "ms";
		glfwSetWindowTitle(mWindow, out.str().c_str());

		elapsedTime = 0.0f;
		frameCount  = 0;
	}

	elapsedTime += dt;
}
