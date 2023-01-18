#pragma once

#include <net_client.hpp>
#include "network/commands.hpp"

class NetworkClient : public net::ClientInterface<NetworkCommands>
{
public:
	void SendChatMessage(std::string message);

	void Listen();
};
