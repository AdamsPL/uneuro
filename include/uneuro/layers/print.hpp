#pragma once

#include <iostream>
#include "layer.hpp"

struct Print : public Layer
{
	template<auto Rows, auto Cols, typename T>
	static const Matrix<Rows, Cols, T> &forward(const Matrix<Rows, Cols, T> &matrix)
	{
		std::cout << matrix << std::endl;
		return matrix;
	}
};

