#include "vectorUtils.h"

std::ostream& operator<< (std::ostream& out, const glm::mat3 & v)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			out << v[j][i] << ",\t";
		out << std::endl;
	}

	return out;
}

std::ostream& operator<< (std::ostream& out, const glm::mat4 & v) {

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			out << v[j][i] << ",\t";
		out << std::endl;
	}

	return out;
}

std::ostream& operator<< (std::ostream& out, const glm::vec2 & v)
{
	out << '['
		<< v[0] << ", "
		<< v[1]
		<< "]";
	return out;
}

std::ostream& operator<< (std::ostream& out, const glm::vec3 & v)
{
	out << '['
		<< v[0] << ", "
		<< v[1] << ", "
		<< v[2]
		<< "]";
	return out;
}

std::ostream& operator<< (std::ostream& out, const glm::vec4 & v)
{
	out << '['
		<< v[0] << ", "
		<< v[1] << ", "
		<< v[2] << ", "
		<< v[3]
		<< "]";
	return out;
}