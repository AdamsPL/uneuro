#pragma once

struct MeanSquaredError
{
	template<typename OutputT>
	constexpr auto calc(const OutputT &output, const OutputT &expected) const
	{
		auto diff = expected - output;
		return (diff.transpose() * diff).sum() / (2 * diff.getColumns());
	}

	template<typename OutputT>
	auto derive(const OutputT &output, const OutputT &expected) const
	{
		return expected - output;
	}
	
};
