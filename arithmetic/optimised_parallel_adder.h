#pragma once
#include <vector>
#include <thread>

class optimised_parallel_adder
{
public:
	static void run_threads(std::vector<std::thread>& threads,
	                        const std::vector<unsigned char>& number1,
	                        const std::vector<unsigned char>& number2,
	                        int threads_count,
	                        std::vector<unsigned char>& sum,
	                        std::vector<unsigned char>& carry_flags);
	static double run(int threads_count,
	                  const std::vector<unsigned char>& number1,
	                  const std::vector<unsigned char>& number2);
};
