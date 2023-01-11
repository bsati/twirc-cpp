#pragma once
#include <string>
#include <asio.hpp>
#include <memory>
#include <vector>
#include "handler.h"
#include "parser.h"

class Client 
{
public:
	Client() 
	{
		running = false;
	}

	void connect(std::string_view nick, std::string_view pass, bool use_ssl);
	void disconnect();
	void add_handler(std::unique_ptr<ChannelEventHandler> handler);
	void add_handler(std::unique_ptr<MessageHandler> handler);
private:
	std::unique_ptr<asio::ip::tcp::socket> socket;
	bool running;
	std::vector<std::unique_ptr<ChannelEventHandler>> channel_handlers;
	std::vector<std::unique_ptr<MessageHandler>> message_handlers;
	Parser parser;

	bool handle_error(const asio::error_code& ec, std::string_view context);
	void main_loop();
	bool write(std::string_view message);
	void dispatch(const ChannelEvent& ce);
	void dispatch(const MessageEvent& me);
};