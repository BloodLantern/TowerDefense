#include "network/server_interface.hpp"

void NetworkServer::OnMessage(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg)
{
	switch (msg.header.id)
	{
		case NetworkCommands::PING:
			client->Send(msg); // Bounce back request
			break;

		case NetworkCommands::CHAT_MESSAGE:
			ShareChatMessage(client, msg);
			break;
	}
}

void NetworkServer::ShareChatMessage(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg)
{
	// Send chat message to everyone
	MessageAllClient(msg);
}
