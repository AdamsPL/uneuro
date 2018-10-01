#pragma once

#include "network.hpp"
#include "gradientDescent.hpp"
#include "utils.hpp"

template<typename T>
struct Typer;

template<typename T>
void getType(T)
{
	Typer<T>::print;
}

template<typename T>
void printType(T&& t)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template <typename NetworkType, typename CostFunctionType>
class Backprop
{
public:
	using WrapperType = decltype(std::declval<NetworkType>().template wrapLayers<GradientDescent>());

	Backprop(NetworkType &net, const CostFunctionType& cost)
		: net(net), wrappers(net.template wrapLayers<GradientDescent>()), cost(cost)
	{
	}

	template <size_t N = 0, typename InputType, typename ExpectedType>
	auto train(const InputType &input, const ExpectedType &expected)
	{
		if constexpr (N == std::tuple_size_v<decltype(wrappers)>) {
			return cost.derive(input, expected);
		} else {
			auto &layer = net.template get<N>();
			auto &wrapper = std::get<N>(wrappers);
			auto output = layer.forward(input);
			auto nextError = train<N+1>(output, expected);

			wrapper.calcGradient(layer, input, nextError);
			return layer.backward(input, nextError);
		}
	}

	template <size_t N = 0>
	void update(float learnRate)
	{
		if constexpr (N < std::tuple_size_v<decltype(wrappers)>) {
			auto &layer = net.template get<N>();
			auto &wrapper = std::get<N>(wrappers);
			wrapper.update(layer, learnRate);
			update<N+1>(learnRate);
		}
	}

private:
	NetworkType &net;
	WrapperType wrappers;
	CostFunctionType cost;
};

