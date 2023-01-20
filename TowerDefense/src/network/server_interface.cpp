#include "network/server_interface.hpp"
#include "globals.hpp"

bool NetworkServer::OnClientConnect(std::shared_ptr<net::Connection<NetworkCommands>> client)
{
	// Check player limit
	if (mPlayerCount >= 2)
		return false; // Don't allow connection

	// Increase player count
	mPlayerCount++;

	// Request username from the client
	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::USERNAME;

	MessageClient(client, msg);

	if (mPlayerCount >= 2)
	{
		net::Message<NetworkCommands> msg;
		msg.header.id = NetworkCommands::PLAYER_CONNECTED;

		Player* player = Globals::gGame->GetPlayerSelf();
		std::string username = player->GetUsername();

		msg.Push(0);
		msg.Push(username.data(), username.size());
		MessageClient(client, msg);
	}

	return true;
}

void NetworkServer::OnClientDisconnect(std::shared_ptr<net::Connection<NetworkCommands>> client)
{
	mPlayerCount--;
	uint32_t id = client->GetID();
	Globals::gGame->RemovePlayer(id);
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

		case NetworkCommands::LEVEL_START:
			// Notify all clients
			MessageAllClient(msg, client);
			break;

		case NetworkCommands::TOWER_PLACED:
			// Notify all clients
			MessageAllClient(msg, client);
			break;

		case NetworkCommands::TOWER_SOLD:
			// Notify all clients
			MessageAllClient(msg, client);
			break;
	}
}

void NetworkServer::ShareChatMessage(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg)
{
	// Send chat message to everyone
	MessageAllClient(msg, client);
}

void NetworkServer::ProcessUsername(std::shared_ptr<net::Connection<NetworkCommands>> client, net::Message<NetworkCommands>& msg)
{
	std::wstring messageW(msg.header.size, '\0');
	msg.Pop(messageW.data(), messageW.size());

	std::string username(messageW.begin(), messageW.end());

	std::string trimUsername;

	for (size_t i = 0; i < username.size(); i++)
	{
		if (username[i] == '\0')
			break;

		trimUsername = trimUsername.append(std::string(1, username[i]));
	}

	Globals::gGame->InstantiatePlayer2(trimUsername, mPlayerCount);
	NotifyClientsOfConnection(client);
}

void NetworkServer::NotifyClientsOfConnection(std::shared_ptr<net::Connection<NetworkCommands>> client)
{
	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::PLAYER_CONNECTED;
	msg.Push(mPlayerCount);

	// Notify the other possible client that someone new has connected
	MessageAllClient(msg, client);
}
