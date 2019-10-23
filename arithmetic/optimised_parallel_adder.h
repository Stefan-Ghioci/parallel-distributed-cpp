#pragma once
#include <vector>
#include <thread>
#include <map>

class optimised_parallel_adder
{
public:
	static void run_threads_for_left_overs(std::vector<std::thread>& threads,
	                                       std::vector<unsigned char>& sum,
	                                       std::map<int, bool>& carry_flags);

	static double run(int threads_count,
	                  const std::vector<unsigned char>& number1,
	                  const std::vector<unsigned char>& number2);
};
