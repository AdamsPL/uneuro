#pragma once

template <typename CostFunctionType, typename NetworkType>
class NetworkTrainer
{
public:
	constexpr NetworkTrainer(const CostFunctionType &cost, NetworkType &network)
		: cost(cost), network(network)
	{
	}

	template<typename InputType, typename OutputType>
	auto train(const InputType &input, const OutputType &expected)
	{
		return network.forward(input);
	}

private:
	CostFunctionType cost;
	NetworkType &network;
};
