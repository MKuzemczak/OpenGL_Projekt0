#include <string>

#include "Triangles.h"

GLuint Triangles::transformationMatrixID;

Triangles::Triangles()
{
	transformationMatrix = glm::mat4(1.0f);
	vertexBufferSize = 1024;
	newVertices = false;
	newLastPoint = false;
	newColors = false;
}


Triangles::~Triangles()
{
	if (vertices.size() == colors.size())
		glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	if(objectCntr == 1)
		glDeleteProgram(programID);
}

void Triangles::generateShaders()
{
	if (programID == NULL)
	{
		std::ofstream vertexShader, fragmentShader;
		vertexShaderFileName.append("object");
		vertexShaderFileName.append(std::to_string(objectNumber));
		vertexShaderFileName.append("VertexShader.vertexshader");
		fragmentShaderFileName.append("object");
		fragmentShaderFileName.append(std::to_string(objectNumber));
		fragmentShaderFileName.append("FragmentShader.fragmentshader");

		vertexShader.open(vertexShaderFileName);
		vertexShader
			<< "#version 330 core\n\n"
			<< "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
			<< "layout(location = 1) in vec4 vertexColor;\n\n"
			<< "out vec4 fragmentColor;\n\n"
			<< "uniform mat4 transformationMatrix;\n\n"
			<< "void main() {\n"
			<< "\tgl_Position =  transformationMatrix * vec4(vertexPosition_modelspace,1);\n"
			<< "\tfragmentColor = vertexColor;\n"
			<< "}";
		vertexShader.close();

		fragmentShader.open(fragmentShaderFileName);
		fragmentShader
			<< "#version 330 core\n\n"
			<< "in vec4 fragmentColor;\n"
			<< "out vec4 color;\n\n"
			<< "void main() {\n"
			<< "\tcolor = fragmentColor;\n"
			<< "}";
		fragmentShader.close();


		programID = LoadShaders(vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

		remove(vertexShaderFileName.c_str());
		remove(fragmentShaderFileName.c_str());

		transformationMatrixID = glGetUniformLocation(programID, "transformationMatrix");
	}

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
}

void Triangles::draw()
{
	if (visible)
	{// Use our shader
		glUseProgram(programID);

		glm::mat4 mvp = orthoMatrix * transformationMatrix;

		//std::cout << "Triangles::draw()::mvp:\n" << mvp << std::endl;

		glUniformMatrix4fv(transformationMatrixID, 1, GL_FALSE, &mvp[0][0]);

		if (newColors)
		{
			GLuint offset = 0,
				size = colors.size() * sizeof(colors[0]);
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, colors.data());
		}

		glEnableVertexAttribArray(0);
		// 1rst attribute buffer : vertices
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(drawingMode, 0, vertices.size() / 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

void Triangles::rotate(float angle)
{
	transformationMatrix[0][0] = cos(angle);
	transformationMatrix[1][0] = -sin(angle);
	transformationMatrix[0][1] = sin(angle);
	transformationMatrix[1][1] = transformationMatrix[0][0];
}

void Triangles::move(glm::vec2 vec)
{
	transformationMatrix[3][0] += vec[0];
	transformationMatrix[3][1] += vec[1];
}

void Triangles::move(glm::vec3 vec)
{
	transformationMatrix[3][0] += vec[0];
	transformationMatrix[3][1] += vec[1];
	transformationMatrix[3][2] += vec[2];
}

void Triangles::setLocation(glm::vec2 point)
{
	transformationMatrix[3][0] = point[0];
	transformationMatrix[3][1] = point[1];
	//std::cout << "Triangles::setLocation::transformationMatrix:\n" << transformationMatrix << std::endl;
}

void Triangles::setLocation(glm::vec3 point)
{
	transformationMatrix[3][0] = point[0];
	transformationMatrix[3][1] = point[1];
	transformationMatrix[3][2] = point[2];
}