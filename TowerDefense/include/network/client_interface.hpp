#pragma once

#include <net_client.hpp>
#include <Point2.h>
#include "network/commands.hpp"
#include "tower.hpp"

class NetworkClient : public net::ClientInterface<NetworkCommands>
{
public:
	void SendUsername();
	void SendChatMessage(std::string message);
	void NotifyStartOfGame(uint8_t level);
	void NotifyTowerPlaced(const Point2 tilePos, const int32_t toweId);
	void NotifyTowerSold(const Point2 tilePos);
	void NotifyTowerUpgrade(const Point2 tilePos, UpgradeType type);

	void Listen();

private:
	void ProcessChatMessage(net::Message<NetworkCommands>& msg);
	void ProcessPlayerConnected(net::Message<NetworkCommands>& msg);
	void ProcessStartOfGame(net::Message<NetworkCommands>& msg);
	void ProcessTowerPlaced(net::Message<NetworkCommands>& msg);
	void ProcessTowerSold(net::Message<NetworkCommands>& msg);
	void ProcessTowerUpgrade(net::Message<NetworkCommands>& msg);
};
