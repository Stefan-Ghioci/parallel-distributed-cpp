#pragma once
#include <string>
#include <vector>
#include <chrono>

class data_utils
{
public:
	static std::string string_array_to_csv_string(const std::vector<std::string>& strings);
	static std::vector<unsigned char> string_to_big_number(const std::string& string);
	static std::string cut_starting_zeroes(const std::string& string);
	static std::string big_number_to_string(const std::vector<unsigned char>& number);
	static void extend_array_memory(std::vector<std::vector<unsigned char>>& numbers);
	static double get_elapsed_time_millis(const std::chrono::steady_clock::time_point& start_time,
	                                      const std::chrono::steady_clock::time_point& end_time);
};
