#pragma once
#include <string>
#include <vector>
#include <regex>
#include "events.h"

std::vector<std::string_view> split(std::string_view input, std::string_view delimiter);

class Parser
{
public:
	MessageEvent parse_message_event(std::string_view message);
	JoinEvent parse_join_event(std::string_view message);
	PartEvent parse_part_event(std::string_view message);
private:
	std::regex message_format = std::regex("^:.*!(.+)@.+ PRIVMSG #(.+) :(.+)\r\n$", std::regex_constants::ECMAScript);
	std::regex join_format = std::regex("^:.*!(.+)@.+ JOIN #(.+)\r\n$", std::regex_constants::ECMAScript);
	std::regex part_format = std::regex("^:.*!(.+)@.+ PART #(.+)\r\n$", std::regex_constants::ECMAScript);

	std::smatch parse_message(const std::regex& format, std::string_view message, int group_count);
};