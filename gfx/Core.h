#pragma once
#include "glew.h"
#include "glfw3.h"
#include "glfw3native.h"
#include <stdio.h>

typedef uint32_t uint;

namespace gfx
{
	struct Color
	{
		float r = 0.f, g = 0.f, b = 0.f;
	};
	struct WindowOptions
	{
		bool resizable = false;
		Color clear = {};
	};
	struct OpenGLInstance
	{
		GLFWwindow* window;

		bool IsRunning() const { return !glfwWindowShouldClose(window); }
	};


	// to be called if any initialization fails
	static OpenGLInstance err(const char* msg, GLFWwindow* window = nullptr)
	{
		printf("%s error, aborting...\n", msg);
		glfwTerminate();
		if (window)
			glfwDestroyWindow(window);
		return { nullptr };
	}
	// to be called at the beginning of the program
	static OpenGLInstance init(uint width, uint height, const char* title, const WindowOptions& options = {})
	{
		// returns false if there's an error
		if (!glfwInit())
			return err("GLFW init");

		// user decides if window is resizable
		glfwWindowHint(GLFW_RESIZABLE, options.resizable);

		// For our purposes, 3.3 is all we need. These four
		// lines are necessary to get it working for everyone.

		// set version to 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		// use 3.3 core profile (no extensions required)
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// if version greater than 3.3 found, also accept that
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


		// attempt to create a window on the primary monitor (first nullptr specifies this)
		GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		glfwMakeContextCurrent(window);

		// glfwCreateWindow can return nullptr if there's an error
		if (!window)
			return err("Window creation");

		// don't remember why we need this, but it's common to need it
		glewExperimental = GL_TRUE;
		// returns true if there's an error
		if (glewInit())
			return err("GLEW init", window);

		// now that GLEW has bound all of our OpenGL functions for us, we can use them

		// background color of the window
		glClearColor(options.clear.r, options.clear.g, options.clear.b, 1.f);
		// intuitive blending mode
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// properly adjust settings when window is resized
		glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) { glViewport(0, 0, width, height); });

		return { window };
	}
	// to be called at the end of the program
	static void end(OpenGLInstance& instance)
	{
		glfwDestroyWindow(instance.window);
		glfwTerminate();
	}
	static int getMaxTextureUnits()
	{
		int result;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &result);
		return result;
	}
}
