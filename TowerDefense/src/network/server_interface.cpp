#include "network/server_interface.hpp"

bool NetworkServer::OnClientConnect(std::shared_ptr<net::Connection<NetworkCommands>> client)
{
	// Check player limit
	if (mPlayerCount >= 4)
		return false; // Don't allow connection

	// Increase player count
	mPlayerCount++;

	// Request username from the client
	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::USERNAME;

	MessageClient(client, msg);

	return true;
}

void NetworkServer::OnClientDisconnect(std::shared_ptr<net::Connection<NetworkCommands>> client)
{
	mPlayerCount--;
}


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
		
		case NetworkCommands::USERNAME:
			// Received username from a client
			ProcessUsername(client, msg);
			break;
	}
}

void NetworkServer::ShareChatMessage(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg)
{
	// Send chat message to everyone
	MessageAllClient(msg);
}

void NetworkServer::ProcessUsername(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg)
{
	std::wstring messageW(msg.header.size, '\0');
	msg.Pop(messageW.data(), messageW.size());

	std::string username(messageW.begin(), messageW.end());

	std::cout << username << std::endl;
}
