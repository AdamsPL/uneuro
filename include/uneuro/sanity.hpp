#pragma once

#include "network.hpp"
#include "layers/linear.hpp"
#include "layers/activation.hpp"

namespace
{

[[maybe_unused]] void testZeroSizedMatrix()
{
	Matrix<0, 0>();
}

[[maybe_unused]] void testSingleElementMatrix()
{
	constexpr Matrix<1, 1> a(5.0f);
	constexpr Matrix<1, 1> b = a;
	constexpr Matrix<1, 1> c = std::move(b);
	(void)c;
}

[[maybe_unused]] void testFeedForward()
{
	constexpr Matrix input {1, 0};
	constexpr auto net = Network(
		Linear<2, 3, int>({1, 2, 3, 4, 5, 6}, {2, 3, 4}),
		Relu());
	constexpr auto result = net.forward(input);
	constexpr Matrix expected {3, 6, 9};

	static_assert(expected == result);
}

[[maybe_unused]] void testNetSize()
{
	constexpr auto net = Network(
		Linear<2, 3, int>({1, 2, 3, 4, 5, 6}, {2, 3, 4}),
		Tanh());
	static_assert(sizeof(net) == sizeof(int) * (6+3+1));
}

[[maybe_unused]] void testColumnCatenation()
{
	constexpr Matrix<2, 3, int> m1 {1, 2, 3,
         		     			    4, 5, 6};
	constexpr Matrix<2, 2, int> m2 {-1, -2,
							        -3, -4};
	constexpr auto result = m1.columnCat(m2);
	constexpr Matrix<2, 5, int> expected {1, 2, 3, -1, -2,
										  4, 5, 6, -3, -4};
	static_assert(result == expected);
}

[[maybe_unused]] void testColumnSplit()
{
	constexpr Matrix<1, 3, int> m1 {1, 2, 3};
	constexpr Matrix<1, 2, int> exp1 {1, 2};
	constexpr Matrix<1, 1, int> exp2 {3};
	constexpr auto tup = m1.columnSplit<2>();

	static_assert(exp1 == std::get<0>(tup));
	static_assert(exp2 == std::get<1>(tup));
}

}
