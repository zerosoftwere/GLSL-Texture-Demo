#pragma once

#include "GLUtil.h"

class GLApp
{
public:
	GLApp(int width, int height, std::string caption);
	virtual ~GLApp(void);

	virtual bool DeviceCheck() { return true; }
	virtual void UpdateScene(float dt) { }
	virtual void DrawScene() { }

	virtual void Run();

	float AspectRatio();
	void  CalculateFrameStats(float dt);
protected:
	GLFWwindow* mWindow;
	std::string mCaption;

	int mWidth, mHeight;
};

