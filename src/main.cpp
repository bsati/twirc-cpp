#include <iostream>
#include <asio.hpp>
#include <string>

int main()
{
	auto host = "irc.chat.twitch.tv";
	auto port_str = "6667";

	// Resolve DNS
	asio::ip::tcp::resolver::query resolver_query(host, port_str, asio::ip::tcp::resolver::query::numeric_service);
	asio::io_service ios;
	asio::ip::tcp::resolver resolver(ios);
	auto it = resolver.resolve(resolver_query);
	auto endpoint = it->endpoint();

	asio::io_context context;

	asio::error_code ec;
	asio::ip::tcp::socket socket(context);

	socket.connect(endpoint, ec);

	if (ec)
	{
		std::cout << "Error connecting:\n" << ec.message() << "\n";
	}
	else
	{
		std::cout << "Connected\n";

		if (socket.is_open())
		{
			while (true) 
			{
				auto bytes_read = socket.available();

				if (bytes_read > 0)
				{
					std::vector<char> buffer(bytes_read);
					socket.read_some(asio::buffer(buffer.data(), buffer.size()));

					for (auto c : buffer) {
						std::cout << c;
					}
				}
			}
		}
	}

	std::cout << "Hello World" << std::endl;

	std::cin;
	return 0;
}