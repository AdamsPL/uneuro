#pragma once

#include "../vector.hpp"

struct Layer
{
	using GradientType = Vector<0, float>;

	void init() const
	{
	}

	void print() const
	{
	}

	template<typename InputT>
	const InputT &forward(const InputT &input) const
	{
		return input;
	}

	template<typename InputT, typename ErrorT>
	const auto &backward(const InputT&, const ErrorT &error) const
	{
		return error;
	}

	template<typename InputT, typename ErrorT>
	const GradientType calcGradient(const InputT &input, const ErrorT &error) const
	{
		return GradientType{};
	}

	void applyGradient(const GradientType &grad)
	{
	}
};
