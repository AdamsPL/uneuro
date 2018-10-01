#include <iostream>

#include <uneuro/network.hpp>
#include <uneuro/sanity.hpp>

__attribute__((noinline)) auto demo()
{
	constexpr Network net {
		Linear<2, 4>{{1.0f, -2.0f, -3.0f, -4.0f, 5.0f, 6.0f, -7.0f, -8.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		Relu{},
		Linear<4, 1>{{-1.0f, -2.0f, 2.0f, 1.0f}, {1.0f}},
		Relu{},
	};
	constexpr Matrix input {1.0f, 1.0f};
	return static_cast<int>(net.forward(input).at(0, 0));
}

int main(int argc, char** argv)
{
	std::cout << demo() << std::endl;
	return 0;
}
