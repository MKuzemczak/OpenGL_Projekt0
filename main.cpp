#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Triangles.h"
#include "Rect.h"
#include "Circ.h"
#include "Curve.h"
#include "shader.hpp"
#include "texture.hpp"

Window * window;

using namespace glm;

// Vertex shader
/*const GLchar* vertexShaderSrc = R"glsl(
    #version 330 core

    in vec2 pos;

    void main()
    {
        gl_Position = vec4(pos, 0.0, 1.0);
    }
	)glsl";

const char* geometryShaderSrc = R"glsl(
    #version 330 core

		layout(points) in;
		layout(line_strip, max_vertices = 2) out;

		void main()
		{
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
	)glsl";

// Fragment shader
const GLchar* fragmentShaderSrc = R"glsl(
    #version 330 core

    out vec4 outColor;
    void main()
    {
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
	)glsl";

// Shader creation helper
GLuint createShader(GLenum type, const GLchar* src) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	return shader;
}*/

int main()
{
	glEnable(GL_DEBUG_OUTPUT);
	window = new Window();
	window->init(800, 800, "Okno");

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	window->enableKeyboard();
	window->setBgcolor(0, 0, 0.4, 0);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////// trojkat
	/*GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);*/
	///////////////////////////////////////////////////////////////////////////////////////////////////// ~trojkat

	

	// Compile and activate shaders
	/*GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSrc);
	GLuint geometryShader = createShader(GL_GEOMETRY_SHADER, geometryShaderSrc);
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);


	// Create VBO with point coordinates
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat points[] = {
		-0.45f,  0.45f,
		0.45f,  0.45f,
		0.45f, -0.45f,
		-0.45f, -0.45f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);*/


	Triangles t;

	t.setDrawingMode(GL_TRIANGLE_FAN);

	std::vector<GLfloat> v = {
		0.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	*(t.verticesVectorPtr()) = v;

	t.generateShaders();

	Rect::setLocationMode(LEFT_CENTER);

	Rect s(0.5f, glm::vec2(-0.25f, 0.25f));
	Rect s0(0.5f, 0.1f, glm::vec2(-0.25f, 0.75f));
	s0.rotate(-PI / 2);
	
	Circ c(0.01f, glm::vec2(-0.25f, 0.75f));
	Circ c0(0.1f, glm::vec2(0.0f, 0.0f));
	Curve cur(0.01);
	cur.addPoint(glm::vec2(0.5f, 0.5f));
	cur.addPoint(glm::vec2(0.5f, 0.3f));
	cur.addPoint(glm::vec2(0.25f, 0.25f));
	
	/*while (1)
	{
		Rect(0.1, 0.1, glm::vec2(0.4, 0.5));
	}*/

	float rot = 0.0;

	do {
		window->clear();

		/////////////////////////////////////////////////////////////// wyswietlanie
		// Use our shader
		/*glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);*/
		////////////////////////////////////////////////////////////// !wyswietlanie

		rot += 0.01;

		c.draw();
		//c0.draw();
		//s0.rotate(rot);
		if (glfwGetMouseButton(window->getGLFWWindowPtr(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window->getGLFWWindowPtr(), &xpos, &ypos);
			//std::cout << xpos << ", " << ypos << std::endl;
			xpos = (2 * xpos / window->width()) - 1;
			ypos = (2 * (window->height() - ypos) / window->height()) - 1;
			cur.addPoint(glm::vec2(xpos, ypos));
		}

		cur.draw();

		//t.draw();
		//s.draw();
		s0.draw();
		//if (window->isPressed(GLFW_KEY_W))
		// Render frame
		//glDrawArrays(GL_POINTS, 0, 4);
		
		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));

	// Cleanup VBO
	/*glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);*/

	window->close();
	glDisable(GL_DEBUG_OUTPUT);
	return 0;
}