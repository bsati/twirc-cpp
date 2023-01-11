#include "client.h"
#include <stdexcept>
#include <iostream>


void Client::connect(std::string_view nick, std::string_view pass, bool use_ssl)
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
	
	write(std::format("PASS {}\r\n", pass));
	write(std::format("NICK {}\r\n", nick));

	this->running = true;
	this->main_loop();
}

void Client::disconnect()
{
	this->running = false;
	this->socket.get()->close();
}

void Client::add_handler(std::unique_ptr<ChannelEventHandler> handler)
{
	this->channel_handlers.push_back(std::move(handler));
}

void Client::add_handler(std::unique_ptr<MessageHandler> handler)
{
	this->message_handlers.push_back(std::move(handler));
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
	if (!this->socket.get() || !this->socket.get()->is_open()) 
	{
		return;
	}

	std::cout << "Connected and main loop started\n";

	while (this->running)
	{
		auto bytes_read = this->socket.get()->available();

		if (bytes_read > 0)
		{
			asio::streambuf buf;
			asio::read_until(*this->socket.get(), buf, "\r\n");
			std::string message = asio::buffer_cast<const char*>(buf.data());

			std::cout << message;

			if (message == "PING :tmi.twitch.tv")
			{
				this->write("PONG :tmi.twitch.tv");
			}
			else
			{
				auto s = split(message, " ");
				if (s.size() < 2) 
				{
					std::cout << "Received malformed message that cannot be parsed: <" << message << ">\n";
				}
				if (s[1] == "PRIVMSG")
				{
					this->dispatch(this->parser.parse_message_event(message));
				}
				else if (s[1] == "JOIN") 
				{
					this->dispatch(this->parser.parse_join_event(message));
				}
				else if (s[1] == "PART") 
				{
					this->dispatch(this->parser.parse_part_event(message));
				}
			}
		}
	}
}

bool Client::write(std::string_view message)
{
	if (!this->socket.get() || !this->socket.get()->is_open())
	{
		return false;
	}
	asio::error_code ec;

	this->socket.get()->write_some(asio::buffer(message.data(), message.size()), ec);

	return handle_error(ec, "Writing Message");
}

void Client::dispatch(const ChannelEvent& ce)
{
	for (auto& handler : this->channel_handlers)
	{
		handler.get()->handle_channel_event(ce);
	}
}

void Client::dispatch(const MessageEvent& me)
{
	for (auto& handler : this->message_handlers)
	{
		handler.get()->handle_message(me);
	}
}
