#include "network/client_interface.hpp"
#include "network/chat_console.hpp"
#include "globals.hpp"
#include "tower.hpp"
#include "network/network_interface.hpp"
#include "spray_projectile.hpp"
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

void NetworkClient::NotifyTowerPlaced(const Point2 tilePos, const int32_t towerId)
{
	if (!IsConnected())
	{
		std::cout << "[Client] server down" << std::endl;
		return;
	}

	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::TOWER_PLACED;

	msg.Push(tilePos);
	msg.Push(towerId);
	Send(msg);
}

void NetworkClient::NotifyTowerSold(const Point2 tilePos)
{
	if (!IsConnected())
	{
		std::cout << "[Client] server down" << std::endl;
		return;
	}

	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::TOWER_SOLD;

	msg.Push(tilePos);
	Send(msg);
}

void NetworkClient::NotifyTowerUpgrade(const Point2 tilePos, UpgradeType type)
{
	if (!IsConnected())
	{
		std::cout << "[Client] server down" << std::endl;
		return;
	}

	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::TOWER_UPGRADE;

	msg.Push(tilePos);
	msg.Push(type);
	Send(msg);
}

void NetworkClient::NotifySprayShot(const Point2 tilePos, const Projectile* proj)
{
	if (!IsConnected())
	{
		std::cout << "[Client] server down" << std::endl;
		return;
	}

	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::SPRAY_SHOT;

	msg.Push(tilePos);
	msg.Push(proj->GetPixelPosition());
	msg.Push(proj->GetVelocity());
	msg.Push(proj->GetDamage());
	msg.Push(proj->GetHitboxRadius());
	msg.Push(proj->GetScale());

	Send(msg);
}

void NetworkClient::NotifyPlayingSpeedChange(int32_t speed)
{
	if (!IsConnected())
	{
		std::cout << "[Client] server down" << std::endl;
		return;
	}

	net::Message<NetworkCommands> msg;
	msg.header.id = NetworkCommands::PLAYING_SPEED_CHANGE;

	msg.Push(speed);

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
			ProcessChatMessage(msg);
			break;

		case NetworkCommands::USERNAME:
			// Username was requested by the server
			SendUsername();
			break;

		case NetworkCommands::PLAYER_CONNECTED:
			ProcessPlayerConnected(msg);
			break;
		
		case NetworkCommands::LEVEL_START:
			ProcessStartOfGame(msg);
			break;

		case NetworkCommands::TOWER_PLACED:
			ProcessTowerPlaced(msg);
			break;

		case NetworkCommands::TOWER_SOLD:
			ProcessTowerSold(msg);
			break;

		case NetworkCommands::TOWER_UPGRADE:
			ProcessTowerUpgrade(msg);
			break;

		case NetworkCommands::SPRAY_SHOT:
			ProcessSprayShot(msg);
			break;

		case NetworkCommands::PLAYING_SPEED_CHANGE:
			ProcessPlayingSpeedChange(msg);
	}
}


void NetworkClient::ProcessChatMessage(net::Message<NetworkCommands>& msg)
{
	std::string message(msg.header.size - sizeof(uint32_t), '\0');
	msg.Pop(message.data(), message.size());

	uint32_t playerId;
	msg.Pop(playerId);

	ChatConsole::AddMessage(playerId, message);
}

void NetworkClient::ProcessPlayerConnected(net::Message<NetworkCommands>& msg)
{
	if (Globals::gNetwork.IsServerStarted())
		return;

	// Register new player
	std::string username(msg.header.size - sizeof(uint32_t), '\0');
	msg.Pop(username.data(), username.size());

	uint32_t uid;
	msg.Pop(uid);

	Globals::gGame->InstantiatePlayer2(username, uid);
}

void NetworkClient::ProcessStartOfGame(net::Message<NetworkCommands>& msg)
{
	uint8_t level;
	msg.Pop(level);

	Globals::gGame->StartLevel(level);
}

void NetworkClient::ProcessTowerPlaced(net::Message<NetworkCommands>& msg)
{
	int32_t type;
	msg.Pop(type);
	Point2 tilePos;
	msg.Pop(tilePos);
	PlayField* playfield = Globals::gGame->GetPlayField();

	Tower* tower = playfield->GetTowerBarUI()->towerTemplates[type]->Clone();
	tower->SetTilePosition(tilePos);

	for (uint8_t x = 0; x < tower->GetWidth(); x++)
		for (uint8_t y = 0; y < tower->GetHeight(); y++)
			playfield->SetClipdataTile(tilePos.x + x, tilePos.y + y, CLIPDATA_TYPE_OCCUPIED);

	Player* player = Globals::gGame->GetPlayerOther();
	tower->SetOwner(player);

	player->GetTowers()->push_back(tower);
}

void NetworkClient::ProcessTowerSold(net::Message<NetworkCommands>& msg)
{
	Point2 tilePos;
	msg.Pop(tilePos);

	Player* player = Globals::gGame->GetPlayerOther();
	std::vector<Tower*>* towers = player->GetTowers();

	for (std::vector<Tower*>::iterator it = towers->begin(); it != towers->end(); it++)
	{
		Tower* tower = *it;
		Point2 pos = tower->GetTilePosition();

		if (tilePos == pos)
		{
			tower->toDelete = true;
			PlayField* pf = Globals::gGame->GetPlayField();

			for (int32_t x = 0; x < tower->GetWidth(); x++)
			{
				for (int32_t y = 0; y < tower->GetHeight(); y++)
				{
					pf->ResetClipdataTile(pos.x + x, pos.y + y);
				}
			}

			break;
		}
	}
}

void NetworkClient::ProcessTowerUpgrade(net::Message<NetworkCommands>& msg)
{
	UpgradeType upgrade;
	msg.Pop(upgrade);
	Point2 tilePos;
	msg.Pop(tilePos);

	Player* player = Globals::gGame->GetPlayerOther();
	std::vector<Tower*>* towers = player->GetTowers();

	for (std::vector<Tower*>::iterator it = towers->begin(); it != towers->end(); it++)
	{
		Tower* tower = *it;
		Point2 pos = tower->GetTilePosition();

		if (tilePos == pos)
		{
			PlayField* pf = Globals::gGame->GetPlayField();

			switch (upgrade)
			{
				case ATTACK_SPEED:
				case RANGE:
					tower->genericUpgradeLevels[upgrade]++;
					tower->UpdateGeneric(upgrade);
					break;

				case CUSTOM:
					tower->customUpgradeLevel++;
					tower->OnCustomUpgrade();
			}

			break;
		}
	}
}

void NetworkClient::ProcessSprayShot(net::Message<NetworkCommands>& msg)
{
	Vector2 scale;
	float_t hitbox;
	uint32_t damage;
	Vector2 velocity;
	Point2 pixelPos;
	Point2 tilePos;

	msg.Pop(scale);
	msg.Pop(hitbox);
	msg.Pop(damage);
	msg.Pop(velocity);
	msg.Pop(pixelPos);
	msg.Pop(tilePos);

	SprayProjectile* proj = new SprayProjectile();
	proj->SetPixelPosition(pixelPos);
	proj->SetVelocity(velocity);
	proj->SetDamage(damage);
	proj->SetHitboxRadius(hitbox);
	proj->SetScale(scale);

	Player* player = Globals::gGame->GetPlayerOther();
	std::vector<Tower*>* towers = player->GetTowers();

	Tower* t = nullptr;
	for (std::vector<Tower*>::iterator it = towers->begin(); it != towers->end(); it++)
	{
		Tower* tower = *it;
		Point2 pos = tower->GetTilePosition();

		if (tilePos == pos)
		{
			tower = t;
			break;
		}
	}

	proj->SetOwner(t);

	Globals::gGame->projectiles.push_back(proj);
}

void NetworkClient::ProcessPlayingSpeedChange(net::Message<NetworkCommands>& msg)
{
	int32_t speed;
	msg.Pop(speed);

	Globals::gGame->SetPlayingSpeed(speed, false);
}
