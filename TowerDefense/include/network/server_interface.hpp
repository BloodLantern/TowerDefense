#pragma once

#include <net_server.hpp>
#include "network/commands.hpp"

class NetworkServer : public net::ServerInterface<NetworkCommands>
{
public:
	NetworkServer(uint16_t port)
		: ServerInterface<NetworkCommands>(port)
	{
		mPlayerCount = 0;
	}

	bool OnClientConnect(std::shared_ptr<net::Connection<NetworkCommands>> client) override;
	void OnClientDisconnect(std::shared_ptr<net::Connection<NetworkCommands>> client) override;

	void OnMessage(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg) override;

private:
	void ShareChatMessage(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg);

	void ProcessUsername(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg);

	uint8_t mPlayerCount;
};
