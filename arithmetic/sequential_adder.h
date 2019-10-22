#pragma once
#include <vector>

class sequential_adder
{
public:

	static void compute(const std::vector<unsigned char>& number1, const std::vector<unsigned char>& number2, std::vector<unsigned char>& sum);
	static double run(const std::vector<unsigned char>& number1, const std::vector<unsigned char>& number2);
};
