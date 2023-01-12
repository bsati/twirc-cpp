#pragma once
#include <string>

struct MessageEvent
{
	std::string author;
	std::string channel;
	std::string message;
};

struct ChannelEvent 
{
	std::string channel;
	std::string user;
	bool part_event;
};