#include "pch.h"

int main()
{
	// 800x600 window called "Title" with a solid blue background
	gfx::OpenGLInstance gl = gfx::init(800, 600, "Title", { .clear = {.b = 1.f } });

	/**
	 * CPU step 1 - create vertex data
	 */
	// create a vertex array to store vertex data and metadata
	uint va;
	glGenVertexArrays(1, &va);
	// bind it so that the following operations will affect it
	glBindVertexArray(va);

	// Create a vertex buffer to store vertex data. It's important
	// that we do this AFTER binding our vertex array (see step 2).
	uint vbo;
	glGenBuffers(1, &vbo);
	// Some test vertex data. Counter clockwise order from bottom left.
	// The order here doesn't matter (because we're using an index buffer),
	// but it simplifies the index ordering (see step 3).
	float vertices[] =
	{
		// bottom left
		-.5f, -.5f,
		// bottom right
		.5f, -.5f,
		// top right
		.5f, .5f,
		// top left
		-.5f, .5f
	};
	// Send our test vertex data to our vertex buffer. Because we're only doing this
	// once, GL_STATIC_DRAW works fine.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, math::arrsize(vertices), vertices, GL_STATIC_DRAW);


	/**
	 * CPU step 2 - describe vertex data
	 */
	// NOTE: this 0 corresponds to the layout(location = 0) in our vertex shader.
	// OpenGL knows that this metadata corresponds to our specific vertex buffer
	// because we have bound a vertex array.
	glEnableVertexAttribArray(0);
	// Operate on index 0. We have 2 elements per vertex. Our data type is floats.
	// Our floats are not normalized. The size of each vertex is 2 floats. This group
	// is not offset from the start of the vertex at all (that is, 0 bytes).
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);


	/**
	 * CPU step 3 - create index data
	 */
	// create an index buffer
	uint ibo;
	glGenBuffers(1, &ibo);
	// Because of the ordering of our vertex data in step 1, this is very simple.
	// We have two triangles, one between the bottom left, bottom right, and top right
	// vertices, and the second between the bottom left, top right, and top left vertices.
	uint indices[] = { 0, 1, 2, 0, 2, 3 };
	// same as step 1, upload our data to the GPU.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, math::arrsize(indices), indices, GL_STATIC_DRAW);

	
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
		"void main() {\n"
		"    gl_Position = vec4(i_Position, 0, 1);\n"
		"}";
	// this sets every pixel to be solid red
	const char* fragmentSource =
		"#version 330 core\n"
		"layout(location = 0) out vec4 o_Color;\n"
		"void main() {\n"
		"    o_Color = vec4(1, 0, 0, 1);\n"
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
		glBindVertexArray(va);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, math::arrlen(indices), GL_UNSIGNED_INT, nullptr);

		// once everything has been drawn, display it on the screen
		glfwSwapBuffers(gl.window);
		// check for input events (mouse, keyboard, etc.)
		glfwPollEvents();
	}


	// clean up
	gfx::end(gl);
	return 0;
}