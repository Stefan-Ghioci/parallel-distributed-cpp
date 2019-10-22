#include "file_utils.h"
#include "data_utils.h"
#include <fstream>

void file_utils::write_data_to_csv_file(const std::string& filename, const std::vector<std::string>& data)
{
	std::ofstream file{filename, std::ios::app};
	const auto line = data_utils::string_array_to_csv_string(data);
	file << line << std::endl;
}

void file_utils::generate_big_data_file(const std::string& filename, const int size, int min, int max)
{
	std::ofstream file{filename, std::ios::trunc};

	for (auto i = 0; i < size; i++)
	{
		const auto random_size = rand() % (max - min + 1) + min;
		const auto first_digit = rand() % 9 + 1;

		file << first_digit;

		for (auto j = 0; j < random_size - 1; j++)
		{
			const auto digit = rand() % 10;
			file << digit;
		}

		file << std::endl;
	}
}

bool file_utils::file_contents_equal(const std::string& filename1, const std::string& filename2)
{
	std::ifstream file1{filename1}, file2{filename2};

	std::string content1((std::istreambuf_iterator<char>(file1)), (std::istreambuf_iterator<char>()));
	std::string content2((std::istreambuf_iterator<char>(file2)), (std::istreambuf_iterator<char>()));

	return content1 == content2;
}

std::vector<std::vector<unsigned char>> file_utils::get_big_number_list_from_file(const std::string& filename)
{
	std::ifstream file{filename};
	std::vector<std::vector<unsigned char>> numbers;
	std::string line;

	while (std::getline(file, line))
	{
		auto number = data_utils::string_to_big_number(line);
		numbers.push_back(number);
	}
	return numbers;
}

void file_utils::save_results_to_csv(const std::string& filename,
                                     const int min_digits,
                                     const int max_digits,
                                     const double sequential_time,
                                     const double parallel_time,
                                     const double optimised_parallel_time,
                                     const int threads_count)
{
	std::vector<std::string> data;

	data.push_back(std::to_string(min_digits));
	data.push_back(std::to_string(max_digits));
	data.push_back(std::to_string(sequential_time));
	data.push_back(std::to_string(parallel_time));
	data.push_back(std::to_string(optimised_parallel_time));
	data.push_back(std::to_string(threads_count));

	write_data_to_csv_file(filename, data);
}

void file_utils::save_big_number_to_file(const std::vector<unsigned char>& number, const std::string& filename)
{
	std::ofstream file{filename, std::ios::trunc};
	file << data_utils::big_number_to_string(number);
}
