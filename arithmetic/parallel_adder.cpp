#include "parallel_adder.h"
#include "paths.h"
#include "file_utils.h"
#include "data_utils.h"
#include <thread>
#include <map>
#include <mutex>

std::mutex mtx;

void adder_function(const std::vector<unsigned char>& number1,
                    const std::vector<unsigned char>& number2,
                    std::vector<unsigned char>& sum,
                    const int left,
                    const int right,
                    std::map<int, bool>& carry_flags)
{
	unsigned char carry = 0;
	for (auto i = left; i < right; i++)
	{
		const auto digit_sum = number1[i] + number2[i] + carry;
		sum[i] = digit_sum % 10;
		carry = digit_sum / 10;
	}
	mtx.lock();
	carry_flags.insert(std::pair<int, bool>(right, carry));
	mtx.unlock();
}

void parallel_adder::run_threads(std::vector<std::thread>& threads,
                                 const std::vector<unsigned char>& number1,
                                 const std::vector<unsigned char>& number2,
                                 const int threads_count,
                                 std::vector<unsigned char>& sum,
                                 std::map<int, bool>& carry_flags)
{
	const auto length = number1.size();
	const auto interval_length = length / threads_count;
	int remainder = length % threads_count;

	for (int i = 0, left, right = 0; i < threads_count; i++, remainder--)
	{
		left = right;
		right = remainder > 0 ? left + interval_length + 1 : left + interval_length;
		
		threads.emplace_back(adder_function,
		                     std::ref(number1),
		                     std::ref(number2),
		                     std::ref(sum),
		                     left,
		                     right,
		                     std::ref(carry_flags));
	}
}


void parallel_adder::join_threads(std::vector<std::thread>& threads)
{
	for (auto& thread : threads)
		if (thread.joinable())
			thread.join();
}

void parallel_adder::add_left_overs(std::vector<unsigned char>& sum,
                                    const std::map<int, bool>& carry_flags)
{
	for (const auto& pair : carry_flags)
	{
		auto i = pair.first;
		auto carry = pair.second;
		while (carry != 0)
		{
			const auto digit_sum = sum[i] + carry;
			sum[i] = digit_sum % 10;
			carry = digit_sum / 10;
			i++;
		}
	}
}

double parallel_adder::run(const int threads_count,
                           const std::vector<unsigned char>& number1,
                           const std::vector<unsigned char>& number2)
{
	std::vector<unsigned char> sum(number1.size() + 1);
	std::map<int, bool> carry_flags;
	std::vector<std::thread> threads;

	const auto start_time = std::chrono::steady_clock::now();

	run_threads(threads, number1, number2, threads_count, sum, carry_flags);
	join_threads(threads);
	add_left_overs(sum, carry_flags);

	const auto end_time = std::chrono::steady_clock::now();
	const auto elapsed_time = data_utils::get_elapsed_time_millis(start_time, end_time);

	file_utils::save_big_number_to_file(sum, parallel_sum_filename);

	return elapsed_time;
}
