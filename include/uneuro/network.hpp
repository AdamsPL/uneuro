#pragma once

#include <cstdlib>
#include <tuple>

#include "backprop.hpp"
#include "utils.hpp"

template <typename... Layers>
class Network
{
public:
	constexpr Network(Layers&&... layers)
		: layers(layers...)
	{
	}

	template <size_t N>
	constexpr auto &get()
	{
		return std::get<N>(layers);
	}

	template<size_t N = 0>
	constexpr void init()
	{
		if constexpr (N < std::tuple_size_v<decltype(layers)>) {
			std::get<N>(layers).init();
			init<N+1>();
		}
	}

	template<size_t N = 0>
	constexpr void print() const
	{
		if constexpr (N < std::tuple_size_v<decltype(layers)>) {
			std::get<N>(layers).print();
			print<N+1>();
		}
	}

	template <size_t N = 0, typename InputType>
	constexpr auto forward(const InputType &input) const
	{
		if constexpr (N == std::tuple_size_v<decltype(layers)>) {
			return input;
		} else {
			return forward<N+1>(std::get<N>(layers).forward(input));
		}
	}

	template <template <typename> class Algorithm>
	std::tuple<Algorithm<Layers>...> wrapLayers()
	{
		return std::tuple<Algorithm<Layers>...>();
	}

private:
	std::tuple<Layers...> layers;
};

template <typename... Layers>
Network(Layers...) -> Network<Layers...>;

