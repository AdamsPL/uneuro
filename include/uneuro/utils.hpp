#pragma once

template <typename T, typename... Types>
void print(const T &obj, Types&&... types)
{
	std::cout << obj << std::endl;
	if constexpr (sizeof...(Types) > 0) {
		print(types...);
	}
}
