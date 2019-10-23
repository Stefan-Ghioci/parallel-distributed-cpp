#pragma once
#include <vector>
#include <thread>
#include <vector>
#include <map>
#include <mutex>

class parallel_adder
{
public:

	static void run_threads(std::vector<std::thread>& threads,
	                        const std::vector<unsigned char>& number1,
	                        const std::vector<unsigned char>& number2,
	                        int threads_count,
	                         std::vector<unsigned char>& sum,
	                         std::map<int, unsigned char>& carry_flags);
	static void join_threads(std::vector<std::thread>& threads);

	static void add_left_overs(std::vector<unsigned char>& sum, const std::map<int, unsigned char>& carry_flags);
	static double run(int threads_count,
	                  const std::vector<unsigned char>& number1,
	                  const std::vector<unsigned char>& number2);
};
