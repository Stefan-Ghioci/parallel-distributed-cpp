#pragma once
#include <vector>
#include <thread>
#include <vector>

class parallel_multiplier
{
public:

	static void run_multiplier_threads(std::vector<std::thread>& threads,
	                                   const std::vector<unsigned char>& multiplicand,
	                                   const std::vector<unsigned char>& multiplier,
	                                   int threads_count,
	                                   std::vector<std::vector<unsigned char>>& partial_products);

	static void run_reducer_threads(std::vector<std::thread>& threads,
	                                const std::vector<std::vector<unsigned char>>& numbers,
	                                std::vector<std::vector<unsigned char>>& reduced_numbers,
	                                int threads_count,
	                                size_t size);

	static double run(const int threads_count,
	                  const std::vector<unsigned char>& number1,
	                  const std::vector<unsigned char>& number2);
};
