#pragma once

#include <net_server.hpp>
#include "network/commands.hpp"

class NetworkServer : public net::ServerInterface<NetworkCommands>
{
public:
	NetworkServer(uint16_t port)
		: ServerInterface<NetworkCommands>(port)
	{ }

	bool OnClientConnect(std::shared_ptr<net::Connection<NetworkCommands>> client) override { return true; }

	void OnMessage(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg) override;

private:
	void ShareChatMessage(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg);
};
