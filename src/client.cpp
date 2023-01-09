#include "client.h"
#include <stdexcept>
#include <iostream>


void Client::connect(std::string_view nick, std::string_view token, bool use_ssl)
{
	if (use_ssl)
		throw std::invalid_argument("SSL currently not supported");

	auto host = "irc.chat.twitch.tv";
	auto port_str = "6667";

	// Resolve DNS
	asio::ip::tcp::resolver::query resolver_query(host, port_str, asio::ip::tcp::resolver::query::numeric_service);
	asio::io_service ios;
	asio::ip::tcp::resolver resolver(ios);
	auto it = resolver.resolve(resolver_query);
	auto endpoint = it->endpoint();

	asio::io_context context;

	this->socket = std::make_unique<asio::ip::tcp::socket>(context);

	asio::error_code ec;
	this->socket.get()->connect(endpoint, ec);

	if (!handle_error(ec, "Connect"))
	{
		throw std::runtime_error("Error connecting to server");
	}

	this->main_loop();
	this->running = true;
}

void Client::disconnect()
{
	this->running = false;
	this->socket.get()->shutdown(asio::ip::tcp::socket::shutdown_both);
	this->socket.get()->close();
}

bool Client::handle_error(const asio::error_code& ec, std::string_view context)
{
	if (ec)
	{
		std::cout << "Error while performing <" << context << ">:\n" << ec.message() << "\n";
		return false;
	}
	return true;
}

void Client::main_loop()
{
	if (!this->socket.get()->is_open()) 
	{
		return;
	}

	std::cout << "Connected and main loop started\n";

	while (this->running)
	{
		auto bytes_read = this->socket.get()->available();

		if (bytes_read > 0)
		{
			std::vector<char> buffer(bytes_read);
			this->socket.get()->read_some(asio::buffer(buffer.data(), buffer.size()));

			for (auto c : buffer) {
				std::cout << c;
			}
		}
	}
}
