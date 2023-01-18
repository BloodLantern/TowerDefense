#pragma once

#include "network/client_interface.hpp"
#include "network/server_interface.hpp"

class NetworkInterface
{
public:
	NetworkInterface();
	~NetworkInterface();

	void StartClient(std::string host);
	void StopClient();

	void StartServer();
	void StopServer();

	void Update();

	bool IsServerStarted();

	NetworkClient* client;
	NetworkServer* server;

private:
	bool mServerStarted;
};

