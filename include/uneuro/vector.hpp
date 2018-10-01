#pragma once

#include "matrix.hpp"

template <size_t Length, typename T = float>
using Vector = Matrix<Length, 1, T>;
