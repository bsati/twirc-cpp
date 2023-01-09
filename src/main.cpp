#include <iostream>
#include "client.h"

int main()
{
	Client client{};

	client.connect("", "", false);

	std::cin;
	client.disconnect();

	return 0;
}