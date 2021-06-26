#include "pch.h"

int main()
{
	// 800x600 window called "Title" with a solid blue background
	gfx::OpenGLInstance gl = gfx::init(800, 600, "ACM Game Engine Dev Workshop Series", { .clear = {.b = 1.f } });




	// virtual method redeclarations in child classes DO override the original method
	// virtual constructor/destructor "redeclarations" DO NOT override the originals, but are called after/before them




	float vertices[] =
	{
		// bottom left
		-.5f, -.5f, 1.f, 0.f, 0.f,
		// bottom right
		 .5f, -.5f, 0.f, 1.f, 0.f,
		// top right
		 .5f,  .5f, 0.f, 0.f, 1.f,
		// top left
		-.5f,  .5f, 1.f, 1.f, 1.f
	};
	// create a VAO with our vertex data, separating each vertex into two groups - 
	// one with 2 elements (x, y) and one with 3 (r, g, b)
	gfx::VertexArray va(math::arrlen(vertices), vertices, { 2, 3 });
	// create an IBO to draw the data in our VAO's VBO
	gfx::IndexBuffer ibo(va);

	
	/**
	 * GPU steps 4+6 - vertex and fragment shaders
	 */
	// create space for our shaders
	uint vertex = glCreateShader(GL_VERTEX_SHADER);
	uint fragment = glCreateShader(GL_FRAGMENT_SHADER);

	// this simply takes the data from our vertex buffer
	// and outputs it (unmodified) as a vec4
	const char* vertexSource =
		"#version 330 core\n"
		"layout(location = 0) in vec2 i_Position;\n"
		"layout(location = 1) in vec3 i_Color;\n"
		"out vec3 v_Color;\n"
		"void main() {\n"
		"    gl_Position = vec4(i_Position, 0, 1);\n"
		"    v_Color = i_Color;\n"
		"}";
	// this sets every pixel to be solid red
	const char* fragmentSource =
		"#version 330 core\n"
		"layout(location = 0) out vec4 o_Color;\n"
		"in vec3 v_Color;\n"
		"void main() {\n"
		"    o_Color = vec4(v_Color, 1);\n"
		"}";

	// give our shaders the source code and compile them
	glShaderSource(vertex, 1, &vertexSource, nullptr);
	glShaderSource(fragment, 1, &fragmentSource, nullptr);
	glCompileShader(vertex);
	glCompileShader(fragment);

	// Check for compilation errors. Eventually, we will print
	// the actual error output, but for now this is enough.
	int result;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
		printf("Vertex shader compilation failed\n");
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
		printf("Fragment shader compilation failed\n");

	// create a program to store both of our shaders
	uint program = glCreateProgram();
	// attach both shaders
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	// finalize the program
	glLinkProgram(program);
	glValidateProgram(program);

	// our shaders are now within our program,
	// so we can delete the originals
	glDeleteShader(vertex);
	glDeleteShader(fragment);


	// game loop
	while (gl.IsRunning())
	{
		// clear the screen at the start of each frame
		glClear(GL_COLOR_BUFFER_BIT);

		// draw everything
		glUseProgram(program);
		va.Bind();
		ibo.Bind();
		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);

		// once everything has been drawn, display it on the screen
		glfwSwapBuffers(gl.window);
		// check for input events (mouse, keyboard, etc.)
		glfwPollEvents();
	}


	// clean up
	gfx::end(gl);
	return 0;
}