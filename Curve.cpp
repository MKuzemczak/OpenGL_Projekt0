#include "Curve.h"

GLuint Curve::programID = 0;
GLuint Curve::curveCntr = 0;
GLuint Curve::vertexColorUniformID = 0;
GLuint Curve::orthoMatrixUniformID = 0;

Curve::Curve(float w, glm::vec4 clr)
{
	width = w;
	drawingMode = GL_TRIANGLE_STRIP;
	//drawingMode = GL_LINE_LOOP;
	curveCntr++;
	color = clr;
	generateShaders();
}


Curve::~Curve()
{
	if (!(--curveCntr))
		glDeleteProgram(programID);

	for (Circ * c : circles)
		delete c;
}


void Curve::draw()
{
	for (Circ * c : circles)
		c->draw();

	// Use our shader
	glUseProgram(programID);

	glUniformMatrix4fv(orthoMatrixUniformID, 1, GL_FALSE, &orthoMatrix[0][0]);
	glUniform4fv(vertexColorUniformID, 1, &color[0]);
	
	glEnableVertexAttribArray(0);
	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		
	// taking care of buffer size, doubling its size
	while (vertices.size() > vertexBufferSize)
	{
		vertexBufferSize *= 2;
		GLuint offset = 0,
			size = vertices.size() * sizeof(vertices[0]);
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize * sizeof(vertices.data()), 0, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices.data());
	}

	if (newVertices)
	{
		GLuint offset = 0,
			size = vertices.size() * sizeof(vertices[0]);

		glFinish();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices.data()+(offset/sizeof(vertices[0])));
		newVertices = 0;
	}
	
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the shape !
	glDrawArrays(drawingMode, 0, vertices.size() / 3);

	glDisableVertexAttribArray(0);

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
		if (glm::length(pt - points[points.size() - 1]) >= width)
		{
			static float prevAngle = 0;

			points.push_back(pt);
			circles[1]->setLocation(pt);

			float dx = pt[0] - points[points.size() - 2][0];
			float dy = pt[1] - points[points.size() - 2][1];
			float length = (sqrt(dy*dy + dx * dx));
			float angle = atan(dy / dx);
			if (dx <= 0 && dy >= 0 && angle < 0||
				dx <= 0 && dy <= 0 && angle > 0)
				angle += PI;
			
			if (abs(angle - prevAngle) > 50 * DEG_TO_RAD)
			{
				circles.push_back(new Circ(width / 2, points[points.size() - 2], color));
			}
			prevAngle = angle;

			glm::mat2 rotation;
			rotation[0][0] = dx / length;
			rotation[1][0] = -dy / length;
			rotation[0][1] = dy / length;
			rotation[1][1] = rotation[0][0];

			glm::vec2 v0, v1;
			v0[0] = 0.0f;
			v0[1] =  width/2;
			v0 = rotation * v0;
			v1[0] = 0.0f;
			v1[1] = -width / 2;
			v1 = rotation * v1;
			vertices.push_back(v0[0] + points[points.size() - 2][0]);
			vertices.push_back(v0[1] + points[points.size() - 2][1]);
			vertices.push_back(0.0f);
			vertices.push_back(v1[0] + points[points.size() - 2][0]);
			vertices.push_back(v1[1] + points[points.size() - 2][1]);
			vertices.push_back(0.0f);
			vertices.push_back(v0[0] + pt[0]);
			vertices.push_back(v0[1] + pt[1]);
			vertices.push_back(0.0f);
			vertices.push_back(v1[0] + pt[0]);
			vertices.push_back(v1[1] + pt[1]);
			vertices.push_back(0.0f);

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
			<< "uniform mat4 orthoMatrix;\n"
			<< "uniform vec4 vertexColor;\n\n"
			<< "out vec4 fragmentColor;\n\n"
			<< "void main() {\n"
			<< "\tgl_Position = orthoMatrix * vec4(vertexPosition_modelspace,1);\n"
			<< "\tfragmentColor = vertexColor;\n"
			<< "}";
		vertexShader.close();

		fragmentShader.open(fragmentShaderFileName);
		fragmentShader
			<< "#version 330 core\n\n"
			<< "in vec4 fragmentColor;"
			<< "out vec4 color;\n\n"
			<< "void main() {\n"
			<< "\tcolor = fragmentColor;"
			<< "}";
		fragmentShader.close();


		programID = LoadShaders(vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

		remove(vertexShaderFileName.c_str());
		remove(fragmentShaderFileName.c_str());

		vertexColorUniformID = glGetUniformLocation(programID, "vertexColor");
		orthoMatrixUniformID = glGetUniformLocation(programID, "orthoMatrix");
	}	
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize * sizeof(GLfloat), 0, GL_DYNAMIC_DRAW);
}
