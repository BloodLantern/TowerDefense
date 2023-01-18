#include "network/network_interface.hpp"

NetworkInterface::NetworkInterface()
{
	client = new NetworkClient(); 

	// Don't initialize server because it's not necessary for every client to have it launched
	server = nullptr;
}

NetworkInterface::~NetworkInterface()
{
	delete client;

	// Delete client if necessary
	if (server)
		delete server;
}

void NetworkInterface::StartClient(std::string host)
{
	// Connect client
	client->Connect(host, 60000);
}

void NetworkInterface::StopClient()
{
	// Disconnect client
	client->Disconnect();
}

void NetworkInterface::StartServer()
{
	// Actually init the server here if necessary
	if (!server)
		server = new NetworkServer(60000);

	// Start it
	server->Start();
	mServerStarted = true;
}

void NetworkInterface::StopServer()
{
	// Stop the server
	server->Stop();

	// Fully delete it
	delete server;
	server = nullptr;
	mServerStarted = false;
}

bool NetworkInterface::IsServerStarted()
{
	return mServerStarted;
}

void NetworkInterface::Update()
{
	if (client->IsConnected())
		client->Listen();

	if (server)
		server->Update();
}
