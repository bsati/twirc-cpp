#include "parser.h"
#include <vector>

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

MessageEvent parse_message_event(std::string_view message)
{
	auto first_space = message.find(' ');
	if (first_space == std::string::npos)
	{

	}
	return MessageEvent();
}

ChannelEvent parse_channel_event(std::string_view message)
{
	return ChannelEvent();
}
