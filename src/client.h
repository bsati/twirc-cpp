#pragma once
#include <string>
#include <asio.hpp>
#include <memory>

class Client 
{
public:
	Client() 
	{
		running = false;
	}

	void connect(std::string_view nick, std::string_view token, bool use_ssl);
	void disconnect();
private:
	std::unique_ptr<asio::ip::tcp::socket> socket;
	bool running;

	bool handle_error(const asio::error_code& ec, std::string_view context);
	void main_loop();
};