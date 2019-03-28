#include <string>

#include "Triangles.h"



Triangles::Triangles()
{
	transformationMatrix = glm::mat4(1.0f);
	vertexBufferSize = 1024;
	newVertices = false;
	newLastPoint = false;
}


Triangles::~Triangles()
{
	if (vertices.size() == colors.size())
		glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteProgram(programID);
}

void Triangles::generateShaders()
{
	std::ofstream vertexShader, fragmentShader;
	vertexShaderFileName.append("object");
	vertexShaderFileName.append(std::to_string(objectNumber));
	vertexShaderFileName.append("VertexShader.vertexshader");
	fragmentShaderFileName.append("object");
	fragmentShaderFileName.append(std::to_string(objectNumber));
	fragmentShaderFileName.append("FragmentShader.fragmentshader");

	vertexShader.open(vertexShaderFileName);
	vertexShader << "#version 330 core"
		<< std::endl
		<< std::endl
		<< "layout(location = 0) in vec3 vertexPosition_modelspace;" << std::endl;
	/*if (vertices.size() == colors.size())
	{
		vertexShader << "layout(location = 1) in vec3 vertexColor;" << std::endl << std::endl
			<< "out vec3 fragmentColor;" << std::endl << std::endl;
	}*/
	vertexShader << "uniform mat4 transformationMatrix;" << std::endl << std::endl
		<< "void main() {" << std::endl
		<< "\tgl_Position =  transformationMatrix * vec4(vertexPosition_modelspace,1);" << std::endl;
	/*if (vertices.size() == colors.size())
		vertexShader << "\tfragmentColor = vertexColor;" << std::endl;*/
	vertexShader << "}";
	vertexShader.close();

	fragmentShader.open(fragmentShaderFileName);
	fragmentShader << "#version 330 core" << std::endl << std::endl;
	/*if (vertices.size() == colors.size())
	{
		fragmentShader << "in vec3 fragmentColor;" << std::endl;
	}*/
	fragmentShader << "out vec3 color;" << std::endl << std::endl
		<< "void main() {" << std::endl;
	/*if (vertices.size() == colors.size())
		fragmentShader << "\tcolor = fragmentColor;" << std::endl;
	else*/
		fragmentShader << "\tcolor = vec3(1,0,0);" << std::endl;
	fragmentShader << "}";
	fragmentShader.close();

	programID = LoadShaders(vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

	remove(vertexShaderFileName.c_str());
	remove(fragmentShaderFileName.c_str());

	transformationMatrixID = glGetUniformLocation(programID, "transformationMatrix");

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	/*if (vertices.size() == colors.size())
	{
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors.data()), colors.data(), GL_STATIC_DRAW);
	}*/
}

void Triangles::draw()
{
	
	// Use our shader
	glUseProgram(programID);

	glUniformMatrix4fv(transformationMatrixID, 1, GL_FALSE, &transformationMatrix[0][0]);

	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	/*if (vertices.size() == colors.size())
	{
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
	}*/
	// Draw the triangle !
	glDrawArrays(drawingMode, 0, vertices.size() / 3);

	glDisableVertexAttribArray(0);
	if (vertices.size() == colors.size())
	{
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
}

void Triangles::setLocation(glm::vec3 point)
{
	transformationMatrix[3][0] = point[0];
	transformationMatrix[3][1] = point[1];
	transformationMatrix[3][2] = point[2];
}