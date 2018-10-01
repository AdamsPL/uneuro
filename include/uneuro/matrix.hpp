#pragma once

#include <array>
#include <iostream>
#include <algorithm>
#include <functional>
#include <random>
#include <tuple>

template <size_t Rows, size_t Cols, typename T = float>
class Matrix
{
public:
	constexpr Matrix() = default;

	template <typename... Types, typename = std::enable_if_t<!std::is_same_v<std::common_type_t<Types...>, Matrix>>>
	constexpr Matrix(Types&&... params)
		: data { std::forward<Types>(params)... }
	{
		static_assert(sizeof...(Types) == Rows * Cols, "Wrong number of arguments!");
	}

	constexpr auto &at(size_t row, size_t col)
	{
		return data[row * Cols + col];
	}

	constexpr auto &at(size_t row, size_t col) const
	{
		return data[row * Cols + col];
	}

	template <typename Operation, typename... OperandTypes>
	constexpr auto map(const Operation &oper, OperandTypes&&... operands) const
	{
		Matrix result;
		for (size_t i = 0; i < data.size(); ++i) {
			result.data[i] = oper(this->data[i], operands.data[i]...);
		}
		return result;
	}

	constexpr Matrix operator+(const Matrix &rhs) const
	{
		return map(std::plus<T>{}, rhs);
	}

	constexpr Matrix operator-(const Matrix &rhs) const
	{
		return map(std::minus<T>{}, rhs);
	}

	constexpr auto operator*(const T &rhs) const
	{
		return map([&rhs](const T &el){ return el * rhs; });
	}


	Matrix &operator-=(const Matrix &rhs)
	{
		for (size_t i = 0; i < data.size(); ++i) {
			data[i] -= rhs.data[i];
		}
		return *this;
	}

	Matrix &operator+=(const Matrix &rhs)
	{
		for (size_t i = 0; i < data.size(); ++i) {
			data[i] -= rhs.data[i];
		}
		return *this;
	}

	template<size_t RhsCols>
	constexpr auto operator*(const Matrix<Cols, RhsCols, T>& rhs) const
	{
		Matrix<Rows, RhsCols, T> result;
		for (size_t r = 0; r < Rows; ++r) {
			for (size_t c = 0; c < RhsCols; ++c) {
				T val {};
				for (size_t i = 0; i < Cols; ++i) {
					val += this->at(r, i) * rhs.at(i, c);
				}
				result.at(r, c) = val;
			}
		}
		return result;
	}

	template <size_t NewRows, size_t NewCols, typename = std::enable_if_t<NewRows * NewCols == Rows * Cols>>
	explicit constexpr operator Matrix<NewRows, NewCols, T>() const
	{
		return Matrix<NewRows, NewCols, T>(data);
	}

	constexpr bool operator==(const Matrix &rhs) const
	{
		bool equal = true;
		for (size_t i = 0; i < data.size(); ++i) {
			equal = equal && data[i] == rhs.data[i];
		}
		return equal;
	}

	constexpr T sum() const
	{
		T result(0.0);
		for (auto el : data)
			result += el;
		return result;
	}

	Matrix<Cols, Rows, T> transpose() const
	{
		Matrix<Cols, Rows, T> result;
		for (size_t r = 0; r < Rows; ++r) {
			for (size_t c = 0; c < Cols; ++c) {
				result.at(c, r) = this->at(r, c);
			}
		}
		return result;
	}

	Matrix &randomize()
	{
		std::random_device randomDevice;
		std::default_random_engine engine(randomDevice());
		T variance = 1.0 / (Rows + Cols);
		std::normal_distribution<T> dist {T(0.0), variance};
		
		for (auto &el : data) {
			el = dist(engine);
		}
		return *this;
	}

	Matrix &fill(T value)
	{
		for (auto &el : data) {
			el = value;
		}
		return *this;
	}

	template <size_t OtherRows, size_t OtherCols>
	constexpr Matrix addWrapped(const Matrix<OtherRows, OtherCols, T> &rhs) const
	{
		Matrix result;
		foreachRowAndCol([&result, this, &rhs](size_t r, size_t c) {
			result.at(r, c) = this->at(r, c) + rhs.at(r % OtherRows, c % OtherCols);
		});
		return result;
	}

	constexpr auto flatten() const
	{
		return Matrix<Rows * Cols, 1, T>(data);
	}

	template<typename Operator>
	constexpr void foreachRowAndCol(const Operator &oper) const
	{
		for (size_t r = 0; r < Rows; ++r) {
			for (size_t c = 0; c < Cols; ++c) {
				oper(r, c);
			}
		}
	}

	template<typename Operator>
	constexpr void foreachRowAndCol(const Operator &oper)
	{
		for (size_t r = 0; r < Rows; ++r) {
			for (size_t c = 0; c < Cols; ++c) {
				oper(r, c);
			}
		}
	}

	template <size_t OtherCols>
	constexpr auto columnCat(const Matrix<Rows, OtherCols, T> &rhs) const
	{
		Matrix<Rows, Cols + OtherCols, T> result;
		for (size_t r = 0; r < Rows; ++r) {
			for (size_t c = 0; c < Cols; ++c) {
				result.at(r, c) = this->at(r, c);
			}
			for (size_t c = 0; c < OtherCols; ++c) {
				result.at(r, Cols + c) = rhs.at(r, c);
			}
		}
		return result;
	}

	template <size_t LeftCols>
	constexpr auto columnSplit() const
	{
		static_assert(Cols > LeftCols);

		constexpr size_t RightCols = Cols - LeftCols;
		Matrix<Rows, LeftCols, T> left;
		Matrix<Rows, RightCols, T> right;

		for (size_t r = 0; r < Rows; ++r) {
			for (size_t c = 0; c < LeftCols; ++c) {
				left.at(r, c) = this->at(r, c);
			}
			for (size_t c = 0; c < RightCols; ++c) {
				right.at(r, c) = this->at(r, LeftCols + c);
			}
		}
		return std::make_tuple(left, right);
	}

	friend std::ostream &operator<<(std::ostream& out, const Matrix& mat)
	{
		for (size_t row = 0; row < Rows; ++row) {
			out << "|";
			out << mat.at(row, 0);
			for (size_t col = 1; col < Cols; ++col) {
				out << ", " << mat.at(row, col);
			}
			out << "|" << std::endl;
		}
		return out;
	}

private:
	std::array<T, Rows * Cols> data {};
};

template <typename... Types>
Matrix(Types&&...) -> Matrix<sizeof...(Types), 1, std::common_type_t<Types...>>;
