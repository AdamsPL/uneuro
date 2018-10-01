#pragma once

#include "vector.hpp"

template <size_t InputSize, size_t OutputSize, typename T = float>
class TrainingSample
{
public:
	TrainingSample(const Vector<InputSize, T>& input, const Vector<OutputSize, T>& expected)
		: input(input), expected(expected)
	{
	}

	const auto &getInput() const
	{
		return input;
	}

	const auto &getExpected() const
	{
		return expected;
	}

private:
	Vector<InputSize, T> input;
	Vector<OutputSize, T> expected;
};
