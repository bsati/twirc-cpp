#include "util.h"

std::vector<std::string_view> split(std::string_view input, std::string_view delimiter, bool include_delimiter)
{
	size_t previous = 0, next = 0;

	std::vector<std::string_view> result{};

	while ((next = input.find(delimiter, previous)) != std::string::npos)
	{
		if (include_delimiter)
		{
			next += delimiter.length();
		}
		result.push_back(input.substr(previous, next - previous));
		previous = next + (include_delimiter ? 0 : delimiter.size());
	}

	result.push_back(input.substr(previous));

	return result;
}

std::string to_lower(std::string_view s)
{
	std::string result(s.data());
	
	for (auto i = 0; i < result.size(); ++i)
	{
		result[i] = tolower(result[i]);
	}

	return result;
}
