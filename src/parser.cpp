#include "parser.h"
#include <vector>
#include <stdexcept>
#include <regex>

std::smatch Parser::parse_message(const std::regex& format, const std::string& message, int group_count)
{
	std::smatch matches;

	if (!std::regex_search(message, matches, format) || matches.size() != group_count + 1)
	{
		throw std::invalid_argument("malformed message");
	}

	return matches;
}

MessageEvent Parser::parse_message_event(std::string_view message)
{
	std::string msg(message);
	auto matches = this->parse_message(this->message_format, msg, 3);

	return {
		matches[1].str(),
		matches[2].str(),
		matches[3].str()
	};
}

ChannelEvent Parser::parse_join_event(std::string_view message)
{
	std::string msg(message);
	auto matches = this->parse_message(this->join_format, msg, 2);

	return {
		matches[1], matches[2], false
	};
}

ChannelEvent Parser::parse_part_event(std::string_view message)
{
	std::string msg(message);
	auto matches = this->parse_message(this->part_format, msg, 2);

	return {
		matches[1], matches[2], true
	};
}
