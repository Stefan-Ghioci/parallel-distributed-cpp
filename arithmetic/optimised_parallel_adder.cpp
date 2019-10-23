#include "optimised_parallel_adder.h"
#include <mutex>
#include "data_utils.h"
#include "file_utils.h"
#include "paths.h"
#include "parallel_adder.h"

std::mutex mutex;

void adder_function(const int thread_id,
                    const std::vector<unsigned char>& number1,
                    const std::vector<unsigned char>& number2,
                    std::vector<unsigned char>& sum,
                    const int left,
                    const int right,
                    std::vector<unsigned char>& carry_flags,
                    std::vector<std::thread>& threads)
{
	int digit_sum;
	unsigned char carry = 0;
	for (auto i = left; i < right; i++)
	{
		digit_sum = number1[i] + number2[i] + carry;
		sum[i] = digit_sum % 10;
		carry = digit_sum / 10;
	}
	carry_flags[thread_id] = carry;
	if (thread_id == 0)
		return;

	auto& previous_thread = threads[thread_id - 1];

	mutex.lock();
	if (carry_flags[thread_id - 1] == 0)
		if (previous_thread.joinable())
			previous_thread.join();
	carry = carry_flags[thread_id - 1];
	if (carry != 0)
	{
		for (auto i = left; i < right; i++)
		{
			digit_sum = sum[i] + carry;
			sum[i] = digit_sum % 10;
			carry = digit_sum / 10;
			if (carry == 0)
				break;
		}
		carry_flags[thread_id] += carry;
	}

	if (unsigned(thread_id) + 1 == carry_flags.size())
		sum[right] = carry_flags[thread_id];
}

void optimised_parallel_adder::run_threads(std::vector<std::thread>& threads,
                                           const std::vector<unsigned char>& number1,
                                           const std::vector<unsigned char>& number2,
                                           const int threads_count,
                                           std::vector<unsigned char>& sum,
                                           std::vector<unsigned char>& carry_flags)
{
	const auto length = number1.size();
	const auto interval_length = length / threads_count;
	int remainder = length % threads_count;

	for (int i = 0, left, right = 0; i < threads_count; i++, remainder--)
	{
		left = right;
		right = remainder > 0 ? left + interval_length + 1 : left + interval_length;

		threads.emplace_back(adder_function,
		                     i,
		                     std::ref(number1),
		                     std::ref(number2),
		                     std::ref(sum),
		                     left,
		                     right,
		                     std::ref(carry_flags),
		                     std::ref(threads));
	}
}

double optimised_parallel_adder::run(const int threads_count,
                                     const std::vector<unsigned char>& number1,
                                     const std::vector<unsigned char>& number2)
{
	std::vector<unsigned char> sum(number1.size() + 1);
	std::vector<unsigned char> carry_flags(threads_count);
	std::vector<std::thread> threads;

	const auto start_time = std::chrono::steady_clock::now();

	run_threads(threads, number1, number2, threads_count, sum, carry_flags);
	parallel_adder::join_threads(threads);

	const auto end_time = std::chrono::steady_clock::now();
	const auto elapsed_time = data_utils::get_elapsed_time_millis(start_time, end_time);

	file_utils::save_big_number_to_file(sum, optimised_parallel_sum_filename);

	return elapsed_time;
}
