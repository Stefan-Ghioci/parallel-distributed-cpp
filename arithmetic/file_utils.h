#pragma once
#include <string>
#include <vector>

class file_utils
{
public:
	static void write_data_to_csv_file(const std::string& filename, const std::vector<std::string>& data);

	static void generate_big_data_file(const std::string& filename, int size, int min, int max);

	static bool file_contents_equal(const std::string& filename1, const std::string& filename2);
	
	static std::vector<std::vector<unsigned char>> get_big_number_list_from_file(const std::string& filename);
	
	static void save_results_to_csv(const std::string& filename,
	                                const int min_digits,
	                                const int max_digits,
	                                const double sequential_time,
	                                const double parallel_time,
	                                const double optimised_parallel_time,
	                                int threads_count);
	static void save_big_number_to_file(const std::vector<unsigned char>& number, const std::string& filename);
};
