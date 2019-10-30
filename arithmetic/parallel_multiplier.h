#pragma once
#include <vector>

class parallel_multiplier
{
public:

	static double run(const int threads_count, const std::vector<unsigned char>& number1, const std::vector<unsigned char>& number2);
};
