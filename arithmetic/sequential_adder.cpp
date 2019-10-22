#include "sequential_adder.h"
#include <chrono>
#include "file_utils.h"
#include "paths.h"
#include "data_utils.h"

void sequential_adder::compute(const std::vector<unsigned char>& number1,
                               const std::vector<unsigned char>& number2,
                               std::vector<unsigned char>& sum)
{
	unsigned char carry = 0;
	for (unsigned int i = 0; i < number1.size(); i++)
	{
		const unsigned char digit_sum = number1[i] + number2[i] + carry;
		sum.push_back(digit_sum % 10);
		carry = digit_sum / 10;
	}
	if (carry != 0) sum.push_back(carry);
}


double sequential_adder::run(const std::vector<unsigned char>& number1, const std::vector<unsigned char>& number2)
{
	std::vector<unsigned char> sum;

	const auto start_time = std::chrono::steady_clock::now();

	compute(number1, number2, sum);

	const auto end_time = std::chrono::steady_clock::now();
	const auto elapsed_time = data_utils::get_elapsed_time_millis(start_time, end_time);

	file_utils::save_big_number_to_file(sum, sequential_sum_filename);

	return elapsed_time;
}
