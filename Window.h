#pragma once

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

class Window
{
	GLFWwindow * window;
public:
	Window();

	int init(int w, int h, const char * name);
	void enableKeyboard();
	void disableKeyboard();
	void enableCursor();
	void hideCursor();
	void disableCursor();
	void enableMouseKeys();
	void disableMouseKeys();

	int height();
	int width();
	GLFWwindow * getGLFWWindowPtr();

	void setCursorPos(int w, int h);
	void setBgcolor(float r, float g, float b, float a);
	void clear();
	void swapBuffers();
	bool isPressed(int key);
	bool shouldClose();

	void close();
	

	~Window();
};

