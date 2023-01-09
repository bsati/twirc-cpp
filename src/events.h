#pragma once
#include <string>

struct MessageEvent
{
	std::string_view author;
	std::string_view channel;
	std::string_view message;
};

struct ChannelEvent 
{
	std::string_view channel;
	std::string_view user;
};

struct JoinEvent : ChannelEvent
{
};

struct PartEvent : ChannelEvent
{
};