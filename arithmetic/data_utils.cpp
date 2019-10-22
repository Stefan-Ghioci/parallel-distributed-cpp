#include "data_utils.h"

std::string data_utils::string_array_to_csv_string(const std::vector<std::string>& strings)
{
	std::string csv;
	for (const auto& string : strings) { csv += string + ","; }
	csv.pop_back();
	return csv;
}
