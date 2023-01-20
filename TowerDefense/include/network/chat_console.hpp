#pragma once
#include <vector>
#include <string>

class ChatConsole
{
public:
	static void Draw();
	static void AddMessage(uint32_t playerId, std::string message);

private:
	static std::vector<std::string> mMessages;
	static char mMessageBuffer[500];
};

