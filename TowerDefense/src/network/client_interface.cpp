#include "network/client_interface.hpp"
#include "network/chat_console.hpp"
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

	// Push string
	msg.Push(message.data(), message.size());

	// Send message
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
			std::string message(msg.header.size, '\0');

			uint32_t playerId;
			msg.Pop(playerId);
			msg.Pop(message.data(), message.size());

			ChatConsole::AddMessage(playerId, message);
			break;
		}

		case NetworkCommands::USERNAME:
			// Username was requested by the server
			SendUsername();
	}
}
