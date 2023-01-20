#pragma once

#include <net_client.hpp>
#include <Point2.h>
#include "network/commands.hpp"


class NetworkClient : public net::ClientInterface<NetworkCommands>
{
public:
	void SendUsername();
	void SendChatMessage(std::string message);
	void NotifyStartOfGame(uint8_t level);
	void NotifyTowerPlaced(const Point2 tilePos, const int32_t toweId);

	void Listen();
};
