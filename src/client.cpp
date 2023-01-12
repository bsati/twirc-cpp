#include "client.h"
#include "util.h"
#include <stdexcept>
#include <iostream>
#ifdef SSL_SUPPORT
#include <asio/ssl.hpp>
#endif


constexpr std::string_view delimiter = "\r\n";

void Client::connect(std::string_view nick, std::string_view pass, bool use_ssl)
{

	constexpr auto host = "irc.chat.twitch.tv";
	constexpr auto ssl_port_str = "6697";
	constexpr auto port_str = "6667";

#ifndef SSL_SUPPORT
	if (use_ssl)
	{
		throw std::invalid_argument("compiled without ssl support");
	}
#endif

	// Resolve DNS
	auto port = use_ssl ? ssl_port_str : port_str;
	asio::ip::tcp::resolver::query resolver_query(host, use_ssl ? ssl_port_str : port_str, asio::ip::tcp::resolver::query::numeric_service);
	asio::io_service ios;
	asio::ip::tcp::resolver resolver(ios);
	auto it = resolver.resolve(resolver_query);
	auto endpoint = it->endpoint();

	if (use_ssl)
	{
#ifdef SSL_SUPPORT
		asio::ssl::context ctx(asio::ssl::context::sslv23);
		ctx.set_verify_mode(asio::ssl::context_base::verify_none);
		this->socket = std::make_unique<asio::ip::tcp::socket>(ctx);
#endif
	}
	else
	{
		// use default context
		asio::io_context ctx;
		this->socket = std::make_unique<asio::ip::tcp::socket>(ctx);
	}


	asio::error_code ec;
	this->socket.get()->connect(endpoint, ec);

	if (!handle_error(ec, "Connect"))
	{
		throw std::runtime_error("Error connecting to server");
	}
	
	write(std::format("PASS {}{}", pass, delimiter));
	write(std::format("NICK {}{}", nick, delimiter));

	this->running = true;

	this->join_channel(to_lower(nick));

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

void Client::join_channel(std::string_view channel)
{
	this->write(std::format("JOIN #{}{}", channel, delimiter));
}

void Client::leave_channel(std::string_view channel)
{
	this->write(std::format("PART #{}{}", channel, delimiter));
}

void Client::send_message(std::string_view channel, std::string_view message)
{
	this->write(std::format("PRIVMSG #{} :{}{}", channel, message, delimiter));
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
		if (this->socket.get()->available() == 0)
		{
			continue;
		}
		asio::streambuf buf;
		auto bytes_read = asio::read_until(*this->socket.get(), buf, "\r\n");
		if (bytes_read > 0) 
		{
			buf.commit(bytes_read);
			std::string message = asio::buffer_cast<const char*>(buf.data());
			std::vector < std::string_view> messages;
			if (message.find("\r\n") != message.rfind("\r\n"))
			{
				messages = split(message, "\r\n", true);
			}
			else
			{
				messages = { message };
			}

			for (auto& message : messages)
			{
				if (message == "PING :tmi.twitch.tv")
				{
					this->write("PONG :tmi.twitch.tv");
				}
				else if (message.size() == 0)
				{
					continue;
				}
				else
				{
					auto s = split(message, " ", false);
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
