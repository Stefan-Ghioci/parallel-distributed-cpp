#pragma once
#include <vector>

class sequential_multiplier
{
public:

	static void add_all_numbers(std::vector<unsigned char>& sum, const std::vector<std::vector<unsigned char>>& numbers);
	static void compute(const std::vector<unsigned char>& number1, const std::vector<unsigned char>& number2, std::vector<unsigned char>&
	                    product);
	static double run(const std::vector<unsigned char>& number1, const std::vector<unsigned char>& number2);
};
