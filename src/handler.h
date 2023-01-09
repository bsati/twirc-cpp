#pragma once
#include "events.h"

class MessageHandler
{
	virtual void handle_message(const MessageEvent& event) = 0;
};

class ChannelEventHandler
{
	virtual void handle_channel_event(const ChannelEvent& event) = 0;
};