#include "sequential_multiplier.h"
#include "data_utils.h"
#include "file_utils.h"
#include "paths.h"
#include <algorithm>
#include "sequential_adder.h"

void sequential_multiplier::add_all_numbers(std::vector<unsigned char>& sum,
                                            const std::vector<std::vector<unsigned char>>& numbers)
{
	std::copy(numbers[0].begin(), numbers[0].end(), sum.begin());

	for (unsigned int i = 1, numbers_size = numbers.size(); i < numbers_size; ++i)
	{
		sequential_adder::compute(sum, numbers[i], sum);
	}
}

void sequential_multiplier::compute(const std::vector<unsigned char>& number1,
                                    const std::vector<unsigned char>& number2,
                                    std::vector<unsigned char>& product)
{
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

	std::vector<std::vector<unsigned char>> partial_products;

	for (unsigned int i = 0; i < multiplier.size(); i++)
	{
		std::vector<unsigned char> partial_product(product.size());

		unsigned char carry = 0;

		for (unsigned int j = 0; j < multiplicand.size(); j++)
		{
			const auto digit_product = multiplicand[j] * multiplier[i] + carry;
			partial_product[i + j] = digit_product % 10;
			carry = digit_product / 10;
		}
		if (carry != 0) partial_product[i + multiplicand.size()] = carry;

		partial_products.push_back(partial_product);
	}

	add_all_numbers(product, partial_products);
}

double sequential_multiplier::run(const std::vector<unsigned char>& number1,
                                  const std::vector<unsigned char>& number2)
{
	const int max_size = number1.size() + number2.size();
	std::vector<unsigned char> product(max_size);

	const auto start_time = std::chrono::steady_clock::now();

	compute(number1, number2, product);

	const auto end_time = std::chrono::steady_clock::now();
	const auto elapsed_time = data_utils::get_elapsed_time_millis(start_time, end_time);

	file_utils::save_big_number_to_file(product, sequential_product_filename);

	return elapsed_time;
}
