#include "Curve.h"

GLuint Curve::programID = 0;
GLuint Curve::curveCntr = 0;

Curve::Curve(double w)
{
	width = w;
	startCirc = NULL;
	endCirc = NULL;
	drawingMode = GL_LINE_LOOP/*GL_TRIANGLE_STRIP*/;
	curveCntr++;
	generateShaders();
}


Curve::~Curve()
{
	if (!(--curveCntr))
		glDeleteProgram(programID);

	delete startCirc;
	delete endCirc;
}


void Curve::draw()
{
	if (startCirc != NULL)
		startCirc->draw();
	if (endCirc != NULL)
		endCirc->draw();

	// Use our shader
	glUseProgram(programID);

	glUniformMatrix4fv(transformationMatrixID, 1, GL_FALSE, &transformationMatrix[0][0]);

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

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "glBufferSubData error: " << err << std::endl;
		}
		newVertices = 0;
	}
	glEnableVertexAttribArray(0);
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
		startCirc = new Circ(width / 2, pt);
		endCirc = new Circ(width / 2, pt);
	}
	else
	{
		if (glm::length(pt - points[points.size() - 1]) > 0.02)
		{
			
			endCirc->setLocation(pt);

			float dx = pt[0] - points[points.size() - 1][0];
			float dy = pt[1] - points[points.size() - 1][1];
			float angle = atan(dy / dx);
			/*if (dx < 0 && dy < 0 ||
				dx < 0 && dy > 0)
				angle = PI + angle;*/
			std::cout << "angle: " << angle << std::endl;
			if (dx <= 0 && dy >= 0 ||
				dx <= 0 && dy <= 0)
				angle += PI;
			std::cout << "angle: " << angle << std::endl;
			float length = (sqrt(dy*dy + dx * dx));
			
			points.push_back(pt);

			glm::mat2 rotation;
			rotation[0][0] = cos(angle);
			rotation[1][0] = -sin(angle);
			rotation[0][1] = sin(angle);
			rotation[1][1] = rotation[0][0];

			std::cout << "cos: " << cos(angle) << ", sin: " << sin(angle) << std::endl;
			/*std::cout << "rotation:\n"
				<< rotation[0][0] << ", "
				<< rotation[1][0] << std::endl
				<< rotation[0][1] << ", "
				<< rotation[1][1] << std::endl;*/

			glm::vec2 v;
			//if (points.size() == 1)
			//{
			//std::cout << "vectors:\n";
			v[0] = 0.0f;
			v[1] =  width/2;
			v = rotation * v;
			//std::cout << v[0] << ", " << v[1] << std::endl;
			v[0] += points[points.size() - 2][0];
			v[1] += points[points.size() - 2][1];
			//std::cout << v[0] << ", " << v[1] << std::endl;
			vertices.push_back(v[0]);
			vertices.push_back(v[1]);
			vertices.push_back(0.0f);
			v[0] = 0.0f;
			v[1] = -width / 2;
			v = rotation * v;
			v[0] += points[points.size() - 2][0];
			v[1] += points[points.size() - 2][1];
			//std::cout << v[0] << ", " << v[1] << std::endl;
			vertices.push_back(v[0]);
			vertices.push_back(v[1]);
			vertices.push_back(0.0f);
			//}
			v[0] = 0.0f;
			v[1] = width / 2;
			v = rotation * v;
			//std::cout << v[0] + pt[0] << ", " << v[1] + pt[1] << std::endl;
			vertices.push_back(v[0] + pt[0]);
			vertices.push_back(v[1] + pt[1]);
			vertices.push_back(0.0f);
			v[0] = 0.0f;
			v[1] = -width / 2;
			v = rotation * v;
			//std::cout << v[0] + pt[0] << ", " << v[1] + pt[1] << std::endl;
			vertices.push_back(v[0] + pt[0]);
			vertices.push_back(v[1] + pt[1]);
			vertices.push_back(0.0f);

			newVertices += 12;
			//std::cout << vertices.size() << std::endl;

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
	vertexShader << "uniform mat4 transformationMatrix;" << std::endl << std::endl
		<< "void main() {" << std::endl
		<< "\tgl_Position =  transformationMatrix * vec4(vertexPosition_modelspace,1);" << std::endl;
	vertexShader << "}";
	vertexShader.close();

	fragmentShader.open(fragmentShaderFileName);
	fragmentShader << "#version 330 core" << std::endl << std::endl;
	fragmentShader << "out vec3 color;" << std::endl << std::endl
		<< "void main() {" << std::endl;
	fragmentShader << "\tcolor = vec3(1,0,0);" << std::endl;
	fragmentShader << "}";
	fragmentShader.close();

	if(curveCntr == 1)
		programID = LoadShaders(vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

	remove(vertexShaderFileName.c_str());
	remove(fragmentShaderFileName.c_str());

	transformationMatrixID = glGetUniformLocation(programID, "transformationMatrix");

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize * sizeof(GLfloat), 0, GL_DYNAMIC_DRAW);
}
