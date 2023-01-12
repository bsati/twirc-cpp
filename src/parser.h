#pragma once
#include <string>
#include <vector>
#include <regex>
#include "events.h"

class Parser
{
public:
	MessageEvent parse_message_event(std::string_view message);
	ChannelEvent parse_join_event(std::string_view message);
	ChannelEvent parse_part_event(std::string_view message);
private:
	std::regex message_format = std::regex("^:.*!(.+)@.+ PRIVMSG #(.+) :([^\\s]+)\r\n$", std::regex_constants::ECMAScript);
	std::regex join_format = std::regex("^:.*!(.+)@.+ JOIN #([^\\s]+)\r\n$", std::regex_constants::ECMAScript);
	std::regex part_format = std::regex("^:.*!(.+)@.+ PART #([^\\s]+)\r\n$", std::regex_constants::ECMAScript);

	std::smatch parse_message(const std::regex& format, const std::string& message, int group_count);
};