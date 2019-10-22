#pragma once
#include <string>
#include <vector>

class file_utils
{
public:
	static void write_data_to_csv_file(const std::string& filename, const std::vector<std::string>& data);

	static void generate_big_data_file(const std::string& filename, int size, int min, int max);

	static bool file_contents_equal(const std::string& filename1, const std::string& filename2);
};
