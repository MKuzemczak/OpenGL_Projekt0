#include "Curve.h"

GLuint Curve::programID = 0;
GLuint Curve::curveCntr = 0;
//GLuint Curve::vertexColorUniformID = 0;
GLuint Curve::orthoMatrixUniformID = 0;

Curve::Curve(float w, glm::vec4 clr)
{
	width = w;
	length = 0;
	drawingMode = GL_TRIANGLE_STRIP;
	//drawingMode = GL_LINE_LOOP;
	curveCntr++;
	color = clr;
	updateAllVertices = false;
	updateAllColors = false;
	generateShaders();
}


Curve::~Curve()
{
	if (!(--curveCntr))
		glDeleteProgram(programID);

	for (Circ * c : circles)
		delete c;
}


void Curve::generateShaders()
{
	if (curveCntr == 1)
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
			<< "layout(location = 1) in vec3 vertexColor;\n\n"
			<< "uniform mat4 orthoMatrix;\n"
			<< "out vec3 fragmentColor;\n\n"
			<< "void main() {\n"
			<< "\tgl_Position = orthoMatrix * vec4(vertexPosition_modelspace,1);\n"
			<< "\tfragmentColor = vertexColor;\n"
			<< "}";
		vertexShader.close();

		fragmentShader.open(fragmentShaderFileName);
		fragmentShader
			<< "#version 330 core\n\n"
			<< "in vec3 fragmentColor;"
			<< "out vec4 color;\n\n"
			<< "void main() {\n"
			<< "\tcolor = vec4(fragmentColor, 1.0f);"
			<< "}";
		fragmentShader.close();


		programID = LoadShaders(vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

		remove(vertexShaderFileName.c_str());
		remove(fragmentShaderFileName.c_str());

		//vertexColorUniformID = glGetUniformLocation(programID, "vertexColor");
		orthoMatrixUniformID = glGetUniformLocation(programID, "orthoMatrix");
	}

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize * sizeof(GLfloat), 0, GL_STREAM_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize * sizeof(GLfloat), 0, GL_STREAM_DRAW);
}

void Curve::draw()
{
	if (visible)
	{
		for (Circ * c : circles)
			c->draw();

		// Use our shader
		glUseProgram(programID);

		glUniformMatrix4fv(orthoMatrixUniformID, 1, GL_FALSE, &orthoMatrix[0][0]);
		//glUniform4fv(vertexColorUniformID, 1, &color[0]);



		// taking care of buffer size, doubling its size
		while (vertices.size() > vertexBufferSize)
		{
			vertexBufferSize *= 2;
			GLuint offset = 0,
				size = vertices.size() * sizeof(vertices[0]);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertexBufferSize * sizeof(vertices.data()), 0, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices.data());
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertexBufferSize * sizeof(colors.data()), 0, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, colors.data());
		}

		if (updateAllVertices)
		{
			newVertices = vertices.size();
			updateAllVertices = false;
		}
		if (newVertices)
		{
			GLuint offset = (vertices.size() - newVertices) * sizeof(vertices[0]),
				size = newVertices * sizeof(vertices[0]);

			glFinish();
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices.data() + (offset / sizeof(vertices[0])));
		}
		if (updateAllColors)
		{
			newVertices = colors.size();
			updateAllColors = false;
		}
		if (newVertices)
		{
			GLuint offset = (colors.size() - newVertices) * sizeof(colors[0]),
				size = newVertices * sizeof(colors[0]);
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, colors.data() + (offset / sizeof(colors[0])));
			newVertices = 0;
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
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		// Draw the shape !
		glDrawArrays(drawingMode, 0, vertices.size() / 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

void Curve::addPoint(glm::vec2 pt)
{
	if (points.size() == 0)
	{
		points.push_back(pt);
		circles.push_back(new Circ(width / 2, pt, color));
		circles.push_back(new Circ(width / 2, pt, color));
	}
	else
	{
		float l = glm::length(pt - points[points.size() - 1]);
		if (l >= width/2)
		{
			//static float prevAngle = 0;

			points.push_back(pt);
			circles[1]->setLocation(pt);

			length += l;

			// adding circles at some larger angles
			/*float angle = atan(dy / dx);
			if (dx <= 0 && dy >= 0 && angle < 0||
				dx <= 0 && dy <= 0 && angle > 0)
				angle += PI;
			
			if (abs(angle - prevAngle) > 50 * DEG_TO_RAD)
			{
				circles.push_back(new Circ(width / 2, points[points.size() - 2], color));
			}
			prevAngle = angle;*/

			std::vector<GLfloat> v = calculateRect(points[points.size() - 2], pt);

			for (GLfloat f : v)
				vertices.push_back(f);

			for (int i = 0; i < 4; i++)
			{
				colors.push_back(color[0]);
				colors.push_back(color[1]);
				colors.push_back(color[2]);
			}

			newVertices += 12;
		}
	}
}


glm::vec2 & Curve::getPoint(int index)
{
	return points[index];
}

const glm::vec2 & Curve::getPoint(int index) const
{
	return points[index];
}

void Curve::setPoint(unsigned int index, glm::vec2 loc)
{
	points[index] = loc;
	std::vector<GLfloat> v;

	if (index > 0)
	{
		v = calculateRect(points[index - 1], points[index]);

		for (unsigned int i = 0; i < v.size(); i++)
			vertices[(index - 1) * 12 + i] = v[i];
	}
	else
	{
		circles[0]->setLocation(glm::vec3(loc, 0.0f));
	}
	if (index < points.size() - 1)
	{
		v = calculateRect(points[index], points[index + 1]);

		for (unsigned int i = 0; i < v.size(); i++)
			vertices[index * 12 + i] = v[i];
	}
	else
	{
		circles[1]->setLocation(glm::vec3(loc, 0.0f));
	}

	updateAllVertices = true;
}

std::vector<GLfloat> Curve::calculateRect(glm::vec2 start, glm::vec2 end)
{
	std::vector<GLfloat> ret;

	glm::vec2 change = end - start;
	float length = glm::length(change);

	glm::mat2 rotation;
	rotation[0][0] = change[0] / length;
	rotation[1][0] = -change[1] / length;
	rotation[0][1] = change[1] / length;
	rotation[1][1] = rotation[0][0];

	glm::vec2 v0, v1;
	v0[0] = 0.0f;
	v0[1] = width / 2;
	v0 = rotation * v0;
	v1[0] = 0.0f;
	v1[1] = -width / 2;
	v1 = rotation * v1;
	ret.push_back(v0[0] + start[0]);
	ret.push_back(v0[1] + start[1]);
	ret.push_back(0.0f);
	ret.push_back(v1[0] + start[0]);
	ret.push_back(v1[1] + start[1]);
	ret.push_back(0.0f);
	ret.push_back(v0[0] + end[0]);
	ret.push_back(v0[1] + end[1]);
	ret.push_back(0.0f);
	ret.push_back(v1[0] + end[0]);
	ret.push_back(v1[1] + end[1]);
	ret.push_back(0.0f);

	return ret;
}

size_t Curve::pointsSize()
{
	return points.size();
}

void Curve::setPointColor(unsigned int  index, glm::vec3 clr)
{
	if (index > 0)
	{
		for (int i = 0; i < 6; i++)
			colors[(index - 1) * 12 + 6 + i] = clr[i % 3];
	}
	else
	{
		circles[0]->setColor(glm::vec4(clr, 1.0f));
	}
	if (index < points.size() - 1)
	{
		for (int i = 0; i < 6; i++)
			colors[index * 12 + i] = clr[i % 3];
	}
	else
	{
		circles[1]->setColor(glm::vec4(clr, 1.0f));
	}

	updateAllColors = true;
}

std::vector<glm::vec2> & Curve::getPoints()
{
	return points;
}