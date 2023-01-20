#include "network/client_interface.hpp"
#include "network/chat_console.hpp"
#include "globals.hpp"
#include "network/network_interface.hpp"
#include <windows.h>
#include <Lmcons.h>

void NetworkClient::SendUsername()
{
	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::USERNAME;

	TCHAR username[UNLEN + 1];
	DWORD usernameLen = UNLEN + 1;
	GetUserName(username, &usernameLen);

	msg.Push(username, usernameLen * sizeof(TCHAR));

	Send(msg);
}

void NetworkClient::SendChatMessage(std::string message)
{
	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::CHAT_MESSAGE;

	uint32_t uid = Globals::gGame->GetAssignedPlayerID();
	// Push sender
	msg.Push(uid);
	// Push string
	msg.Push(message.data(), message.size());

	// Send message
	Send(msg);
}

void NetworkClient::NotifyStartOfGame(uint8_t level)
{
	if (!IsConnected())
	{
		std::cout << "[Client] server down" << std::endl;
		return;
	}

	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::LEVEL_START;

	msg.Push(level);
	Send(msg);
}

void NetworkClient::Listen()
{
	if (!IsConnected())
	{
		std::cout << "[Client] server down" << std::endl;
		return;
	}

	if (IncomingMessages().Empty())
		return;

	net::Message<NetworkCommands> msg = IncomingMessages().PopFront().msg;

	switch (msg.header.id)
	{
		case NetworkCommands::PING:
			break;

		case NetworkCommands::CHAT_MESSAGE:
		{
			std::string message(msg.header.size - sizeof(uint32_t), '\0');
			msg.Pop(message.data(), message.size());

			uint32_t playerId;
			msg.Pop(playerId);

			ChatConsole::AddMessage(playerId, message);
			break;
		}

		case NetworkCommands::USERNAME:
			// Username was requested by the server
			SendUsername();
			break;

		case NetworkCommands::PLAYER_CONNECTED:
		{
			if (Globals::gNetwork.IsServerStarted())
				return;

			// Register new player
			std::string username(msg.header.size - sizeof(uint32_t), '\0');
			msg.Pop(username.data(), username.size());

			uint32_t uid;
			msg.Pop(uid);

			Globals::gGame->InstantiatePlayer2(username, uid);
			break;
		}

		case NetworkCommands::LEVEL_START:
		{
			uint8_t level;
			msg.Pop(level);

			Globals::gGame->StartLevel(level);
			break;
		}
	}
}
