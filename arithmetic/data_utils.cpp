#include "data_utils.h"
#include <chrono>

std::string data_utils::string_array_to_csv_string(const std::vector<std::string>& strings)
{
	std::string csv;
	for (const auto& string : strings) { csv += string + ","; }
	csv.pop_back();
	return csv;
}

std::vector<unsigned char> data_utils::string_to_big_number(const std::string& string)
{
	auto length = string.length();
	std::vector<unsigned char> number;
	for (int i = string.size() - 1; i >= 0; i--) { number.push_back(string[i] - '0'); }
	return number;
}

std::string data_utils::cut_starting_zeroes(const std::string& string)
{
	auto i = 0;
	while (string[i] == '0')
		i++;
	return string.substr(i);
}

std::string data_utils::big_number_to_string(const std::vector<unsigned char>& number)
{
	std::string string;
	for (int i = number.size() - 1; i >= 0; i--) { string += std::to_string(number[i]); }
	return cut_starting_zeroes(string);
}

void data_utils::extend_array_memory(std::vector<std::vector<unsigned char>>& numbers)
{
	unsigned int max_length = 0;
	for (const auto& number : numbers)
		if (number.size() > max_length)
			max_length = number.size();

	for (auto& number : numbers)
		while (number.size() != max_length)
			number.push_back(0);
}

double data_utils::get_elapsed_time_millis(const std::chrono::steady_clock::time_point& start_time,
                                           const std::chrono::steady_clock::time_point& end_time)
{
	return double(std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count()) / 1000;
}
