#pragma once
#include <string>
#include "events.h"

MessageEvent parse_message_event(std::string_view message);
ChannelEvent parse_channel_event(std::string_view message);