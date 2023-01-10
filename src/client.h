#pragma once
#include <string>
#include <asio.hpp>
#include <memory>
#include <vector>
#include "handler.h"

class Client 
{
public:
	Client() 
	{
		running = false;
	}

	void connect(std::string_view nick, std::string_view pass, bool use_ssl);
	void disconnect();
private:
	std::unique_ptr<asio::ip::tcp::socket> socket;
	bool running;
	std::vector<ChannelEventHandler> channel_handlers;
	std::vector<MessageHandler> message_handlers;

	bool handle_error(const asio::error_code& ec, std::string_view context);
	void main_loop();
	bool write(std::string_view message);
};