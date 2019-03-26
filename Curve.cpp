#include "Curve.h"



Curve::Curve(double w)
{
	width = w;
	startCirc = NULL;
	endCirc = NULL;
	drawingMode = GL_TRIANGLE_STRIP;
	generateShaders();
}


Curve::~Curve()
{
	delete startCirc;
	delete endCirc;
}


void Curve::draw()
{
	if (startCirc != NULL)
		startCirc->draw();
	if (endCirc != NULL)
		endCirc->draw();
	Triangles::draw();
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
		if (glm::length(pt - points[points.size() - 1]) > 0.01)
		{
			points.push_back(pt);
			endCirc->setLocation(pt);

			float dx = pt[0] - points[points.size() - 2][0];
			float dy = pt[1] - points[points.size() - 2][1];
			float angle = atan(dy / dx);
			/*if (dx < 0 && dy < 0 ||
				dx < 0 && dy > 0)
				angle = PI + angle;*/
			if (angle < 0)
				angle = 2 * PI + angle;
			//std::cout << "angle: " << angle << std::endl;
			double length(sqrt(dy*dy + dx * dx));
			
			glm::mat2 rotation;
			rotation[0][0] = cos(angle);
			rotation[1][0] = -sin(angle);
			rotation[0][1] = sin(angle);
			rotation[1][1] = rotation[0][0];

			//std::cout << "cos: " << cos(angle) << ", sin: " << sin(angle) << std::endl;
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

			newVertices = true;
			//std::cout << vertices.size() << std::endl;

			/*glDeleteBuffers(1, &vertexBuffer);
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);*/
			//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.data()), vertices.data(), 0);
			//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.data()), vertices.data(), GL_DYNAMIC_DRAW);
			/*float *tab = new float[vertices.size()];
			glGetBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size(), tab);
			std::cout << "vertices data:\n";
			for(int i = 0; i < vertices.size()/3; i++)
				std::cout << vertices[i * 3 + 0] << ", " << vertices[i * 3 + 0] << ", " << vertices[i * 3 + 0] << std::endl;
			std::cout << "buffer data:\n";
			for (int i = 0; i < vertices.size()/3; i++)
			{
				std::cout << tab[i * 3 + 0] << ", " << tab[i * 3 + 0] << ", " << tab[i * 3 + 0] << std::endl;
			}*/
			//glGetBufferData
			//glBufferData()
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
