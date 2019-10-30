#include <cstdlib>
#include "file_utils.h"
#include <iostream>
#include "data_utils.h"
#include "sequential_adder.h"
#include "paths.h"
#include "parallel_adder.h"
#include "optimised_parallel_adder.h"
#include "sequential_multiplier.h"
#include "parallel_multiplier.h"


int main(int argc, char* argv[])
{
	const auto threads_count = atoi(argv[1]); // NOLINT(cert-err34-c)
	const auto min_digits = atoi(argv[2]); // NOLINT(cert-err34-c)
	const auto max_digits = atoi(argv[3]); // NOLINT(cert-err34-c)

	srand(time(nullptr)); // NOLINT
	file_utils::generate_big_data_file(numbers_filename, 2, min_digits, max_digits);
	auto numbers = file_utils::get_big_number_list_from_file(numbers_filename);

	const auto number1 = numbers[0];
	const auto number2 = numbers[1];

	const auto sequential_time = sequential_multiplier::run(number1, number2);
	const auto parallel_time = parallel_multiplier::run(threads_count, number1, number2);

	if (file_utils::file_contents_equal(sequential_product_filename, parallel_product_filename))
	{
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "Sequential Time: " << std::endl;
		std::cout << sequential_time << "ms" << std::endl;

		std::cout << "Parallel Time: " << std::endl;
		std::cout << parallel_time << "ms" << std::endl;
		std::cout << "-------------------------------------" << std::endl;

		file_utils::save_results_to_csv(results_filename,
		                                min_digits,
		                                max_digits,
		                                sequential_time,
		                                parallel_time,
		                                threads_count);
	}
	else { std::cout << "Results are not equal!!!" << std::endl; }
}
