#pragma once

#include "../vector.hpp"
#include "../matrix.hpp"
#include "layer.hpp"

#include "../utils.hpp"

template <size_t InputLen, size_t OutputLen, typename T = float>
class Linear : public Layer
{
public:
	using GradientType = Matrix<OutputLen, InputLen + 1>;

	constexpr Linear() = default;

	constexpr Linear(const Matrix<OutputLen, InputLen, T> &weights, const Vector<OutputLen, T> &bias)
		: weights(weights), bias(bias)
	{
	}

	constexpr Linear& setWeights(const Matrix<OutputLen, InputLen, T> &newWeights)
	{
		weights = newWeights;
		return *this;
	}

	constexpr const auto &getWeights() const
	{
		return weights;
	}

	void init()
	{
		weights.randomize();
		bias.randomize();
	}

	template <typename InputType>
	constexpr auto forward(const InputType& input) const
	{
		return (weights * input).addWrapped(bias);
	}

	template<typename InputT, typename ErrorT>
	constexpr auto backward(const InputT& input, const ErrorT& nextError) const
	{
		return weights.transpose() * nextError;
	}

	template<typename InputT, typename ErrorT>
	constexpr GradientType calcGradient(const InputT& input, const ErrorT& nextError) const
	{
		return (nextError * input.transpose()).columnCat(nextError);
	}

	constexpr void applyGradient(const GradientType &gradient)
	{
		auto [weightGrad, biasGrad] = gradient.template columnSplit<InputLen>();
		weights += weightGrad;
		bias += biasGrad;
	}

	void print() const
	{
		::print("WEIGHTS:", weights);
		::print("BIAS", bias);
	}

protected:
	Matrix<OutputLen, InputLen, T> weights;
	Vector<OutputLen, T> bias;
};

