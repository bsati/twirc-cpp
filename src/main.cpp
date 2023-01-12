#include <iostream>
#include <regex>
#include <string>
#include <memory>
#include "client.h"
#include "handler.h"

class LogHandler : public MessageHandler
{
public:
	LogHandler() {}
	~LogHandler() {}

	virtual void handle_message(const MessageEvent& event) override 
	{
		std::cout << std::format("#{} [{}]: {}\n", event.channel, event.author, event.message);
	}
};

class ChannelLogHandler : public ChannelEventHandler
{
public:
	virtual void handle_channel_event(const ChannelEvent& event) override
	{
		std::cout << std::format("[{}] {} #{}\n", event.user, event.part_event ? "parted" : "joined", event.channel);
	}
};

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Please specify a nickname and pass for connecting\n";
		return -1;
	}

	Client client{};

	auto handler = std::make_unique<LogHandler>();
	client.add_handler(std::move(handler));

	auto ce_handler = std::make_unique<ChannelLogHandler>();
	client.add_handler(std::move(ce_handler));

	client.connect(argv[1], argv[2], false);

	std::cin;
	client.disconnect();

	return 0;
}