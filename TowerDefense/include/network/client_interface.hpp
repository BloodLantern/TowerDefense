#pragma once

#include <net_client.hpp>
#include "network/commands.hpp"

class NetworkClient : public net::ClientInterface<NetworkCommands>
{
public:
	void SendUsername();
	void SendChatMessage(std::string message);
	void NotifyStartOfGame(uint8_t level);

	void Listen();
};
