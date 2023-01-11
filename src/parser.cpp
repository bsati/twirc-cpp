#include "parser.h"
#include <vector>
#include <stdexcept>
#include <regex>

std::vector<std::string_view> split(std::string_view input, std::string_view delimiter)
{
	size_t previous = 0, next = 0;

	std::vector<std::string_view> result{};

	while ((next = input.find(delimiter, previous)) != std::string::npos)
	{
		result.push_back(input.substr(previous, next - previous));
		previous = next + 1;
	}

	result.push_back(input.substr(previous));

	return result;
}



std::smatch Parser::parse_message(const std::regex& format, std::string_view message, int group_count)
{
	std::smatch matches;
	std::string msg(message);

	if (!std::regex_search(msg, matches, format) || matches.size() != group_count)
	{
		throw std::invalid_argument("malformed message");
	}

	return matches;
}

MessageEvent Parser::parse_message_event(std::string_view message)
{
	auto matches = this->parse_message(this->message_format, message, 3);

	return {
		matches[1].str(),
		matches[2].str(),
		matches[3].str()
	};
}

JoinEvent Parser::parse_join_event(std::string_view message)
{
	
	auto matches = this->parse_message(this->join_format, message, 2);

	return {
		matches[1], matches[2]
	};
}

PartEvent Parser::parse_part_event(std::string_view message)
{
	auto matches = this->parse_message(this->part_format, message, 2);

	return {
		matches[1], matches[2]
	};
}
