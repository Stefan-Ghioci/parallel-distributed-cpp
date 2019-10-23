#include "optimised_parallel_adder.h"
#include <mutex>
#include "data_utils.h"
#include "file_utils.h"
#include "paths.h"
#include "parallel_adder.h"


void left_over_function(std::vector<unsigned char>& sum, int i, bool carry)
{
	while (carry != 0)
	{
		const auto digit_sum = sum[i] + carry;
		sum[i] = digit_sum % 10;
		carry = digit_sum / 10;
		i++;
	}
}

void optimised_parallel_adder::run_threads_for_left_overs(std::vector<std::thread>& threads,
                                                          std::vector<unsigned char>& sum,
                                                          std::map<int, bool>& carry_flags)
{
	for (auto& pair : carry_flags)
		if (pair.second != 0)
			threads.emplace_back(left_over_function, std::ref(sum), pair.first, pair.second);
}

double optimised_parallel_adder::run(const int threads_count,
                                     const std::vector<unsigned char>& number1,
                                     const std::vector<unsigned char>& number2)
{
	std::vector<unsigned char> sum(number1.size() + 1);
	std::map<int, bool> carry_flags;
	std::vector<std::thread> threads;

	const auto start_time = std::chrono::steady_clock::now();

	parallel_adder::run_threads(threads, number1, number2, threads_count, sum, carry_flags);
	parallel_adder::join_threads(threads);

	threads.clear();

	run_threads_for_left_overs(threads, sum, carry_flags);
	parallel_adder::join_threads(threads);

	const auto end_time = std::chrono::steady_clock::now();
	const auto elapsed_time = data_utils::get_elapsed_time_millis(start_time, end_time);

	file_utils::save_big_number_to_file(sum, optimised_parallel_sum_filename);

	return elapsed_time;
}
