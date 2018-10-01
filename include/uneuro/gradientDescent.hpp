#pragma once

#include <type_traits>

template <typename LayerType>
class GradientDescent
{
public:
	using GradientType = typename LayerType::GradientType;

	GradientDescent()
	{
		gradient.fill(0.0);
	}

	template<typename InputT, typename ErrorT>
	void calcGradient(const LayerType &layer, const InputT& input, const ErrorT& nextError)
	{
		gradient += layer.calcGradient(input, nextError);
	}

	void update(LayerType &layer, float learnRate)
	{
		layer.applyGradient(gradient * learnRate);
		//gradient = gradient * 0.01f;
		gradient.fill(0.0);
	}

	const auto &getGradient() const
	{
		return gradient;
	}

private:
	GradientType gradient;
};
