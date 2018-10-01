#pragma once

#include <cmath>

template <typename Activation>
struct ActivationLayer : public Layer, public Activation
{
	using Activation::Activation;

	template<auto Rows, auto Cols, typename T>
	constexpr Matrix<Rows, Cols, T> forward(const Matrix<Rows, Cols, T> &matrix) const
	{
		Matrix<Rows, Cols, T> result;
		for (size_t r = 0; r < Rows; ++r) {
			for (size_t c = 0; c < Cols; ++c) {
				result.at(r, c) = this->apply(matrix.at(r, c));
			}
		}
		return result;
	}

	template<auto Rows, auto Cols, typename T>
	auto backward(const Matrix<Rows, Cols, T>& input, const Matrix<Rows, Cols, T>& nextError) const
	{
		Matrix<Rows, Cols, T> result;
		for (size_t r = 0; r < Rows; ++r) {
			for (size_t c = 0; c < Cols; ++c) {
				result.at(r, c) = this->derive(input.at(r, c)) * nextError.at(r, c);
			}
		}
		return result;
	}
};

struct TanhActivation
{
	template<typename T>
	constexpr T apply(T x) const
	{
		T ex = std::exp(x);
		T minex = std::exp(-x);
		return (ex - minex) / (ex + minex);
	}

	template<typename T>
	constexpr T derive(T x) const
	{
		T fx = apply(x);
		return 1 - fx * fx;
	}
};

class LeakyReluActivation
{
public:
	constexpr LeakyReluActivation(float alpha = 0.01f)
		: alpha(alpha)
	{
	}

	template<typename T>
	constexpr T apply(T x) const
	{
		return (x > T(0.0) ? x : T(alpha) * x);
	}

	template<typename T>
	constexpr T derive(T x) const
	{
		return (x > T(0.0) ? T(1.0) : T(alpha));
	}

private:
	const float alpha;
};

struct ReluActivation : public LeakyReluActivation
{
	constexpr ReluActivation()
		: LeakyReluActivation(0.0f)
	{
	}
};

using Tanh = ActivationLayer<TanhActivation>;
using Relu = ActivationLayer<ReluActivation>;
using LeakyRelu = ActivationLayer<LeakyReluActivation>;

