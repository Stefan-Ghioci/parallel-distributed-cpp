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
