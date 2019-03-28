#pragma once

#include <vector>
#include <iostream>
#include "glm\glm.hpp"

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
	if (!v.empty()) {
		out << '[';

		for (T element : v)
			out << element << ", ";

		out << "\b\b]";
	}
	return out;
}