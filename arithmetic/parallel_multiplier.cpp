#include "parallel_multiplier.h"
#include "paths.h"
#include "file_utils.h"
#include <chrono>
#include "data_utils.h"
#include <thread>
#include <mutex>
#include "parallel_adder.h"
#include "sequential_multiplier.h"
#include <algorithm>
#include "sequential_adder.h"


std::mutex mutex;

void multiplier_function(const std::vector<unsigned char>& multiplicand,
                         const std::vector<unsigned char>& multiplier,
                         const int left,
                         const int right,
                         std::vector<std::vector<unsigned char>>& partial_products)
{
	for (auto i = left; i < right; i++)
	{
		std::vector<unsigned char> partial_product(multiplicand.size() + multiplier.size());

		unsigned char carry = 0;

		for (unsigned int j = 0; j < multiplicand.size(); j++)
		{
			const auto digit_product = multiplicand[j] * multiplier[i] + carry;
			partial_product[i + j] = digit_product % 10;
			carry = digit_product / 10;
		}
		if (carry != 0) partial_product[i + multiplicand.size()] = carry;

		mutex.lock();
		partial_products.push_back(partial_product);
		mutex.unlock();
	}
}

void reducer_function(const std::vector<std::vector<unsigned char>>& numbers,
                      std::vector<std::vector<unsigned char>>& reduced_numbers,
                      const int left,
                      const int right)
{
	std::vector<unsigned char> sum(numbers[left].size());

	std::copy(numbers[left].begin(), numbers[left].end(), sum.begin());

	for (auto i = left + 1; i < right; i++)
		sequential_adder::compute(sum, numbers[i], sum);

	mutex.lock();
	reduced_numbers.push_back(sum);
	mutex.unlock();
}

void parallel_multiplier::run_multiplier_threads(std::vector<std::thread>& threads,
                                                 const std::vector<unsigned char>& multiplicand,
                                                 const std::vector<unsigned char>& multiplier,
                                                 const int threads_count,
                                                 std::vector<std::vector<unsigned char>>& partial_products)
{
	const auto length = multiplier.size();
	const auto interval_length = length / threads_count;
	int remainder = length % threads_count;

	for (int i = 0, left, right = 0; i < threads_count; i++, remainder--)
	{
		left = right;
		right = remainder > 0 ? left + interval_length + 1 : left + interval_length;

		threads.emplace_back(multiplier_function,
		                     std::ref(multiplicand),
		                     std::ref(multiplier),
		                     left,
		                     right,
		                     std::ref(partial_products));
	}
}

void parallel_multiplier::run_reducer_threads(std::vector<std::thread>& threads,
                                              const std::vector<std::vector<unsigned char>>& numbers,
                                              std::vector<std::vector<unsigned char>>& reduced_numbers,
                                              const int threads_count,
                                              const size_t size)
{
	const auto length = size;
	const auto interval_length = length / threads_count;
	int remainder = length % threads_count;

	for (int i = 0, left, right = 0; i < threads_count; i++, remainder--)
	{
		left = right;
		right = remainder > 0 ? left + interval_length + 1 : left + interval_length;


		threads.emplace_back(reducer_function,
		                     std::ref(numbers),
		                     std::ref(reduced_numbers),
		                     left,
		                     right);
	}
}

double parallel_multiplier::run(const int threads_count,
                                const std::vector<unsigned char>& number1,
                                const std::vector<unsigned char>& number2)
{
	const int max_size = number1.size() + number2.size();

	std::vector<unsigned char> multiplier, multiplicand;

	if (number2.size() < number1.size())
	{
		multiplicand = number1;
		multiplier = number2;
	}
	else
	{
		multiplicand = number2;
		multiplier = number1;
	}

	std::vector<unsigned char> product(max_size);
	std::vector<std::vector<unsigned char>> partial_products;
	std::vector<std::vector<unsigned char>> reduced_partial_products;
	std::vector<std::thread> threads;

	const auto start_time = std::chrono::steady_clock::now();

	run_multiplier_threads(threads, multiplicand, multiplier, threads_count, partial_products);
	parallel_adder::join_threads(threads);

	do
	{
		reduced_partial_products.clear();
		threads.clear();

		run_reducer_threads(threads, partial_products, reduced_partial_products, threads_count, multiplier.size());
		parallel_adder::join_threads(threads);

		partial_products.clear();
		partial_products.resize(reduced_partial_products.size());
		std::copy(reduced_partial_products.begin(), reduced_partial_products.end(), partial_products.begin());
	}
	while (2 * threads_count <= reduced_partial_products.size());

	sequential_multiplier::add_all_numbers(product, partial_products);

	const auto end_time = std::chrono::steady_clock::now();
	const auto elapsed_time = data_utils::get_elapsed_time_millis(start_time, end_time);

	file_utils::save_big_number_to_file(product, parallel_product_filename);

	return elapsed_time;
}
