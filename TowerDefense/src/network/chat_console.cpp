#include "network/chat_console.hpp"
#include "network/network_interface.hpp"
#include "imgui.h"
#include "globals.hpp"
#include "gui.hpp"

std::vector<std::string> ChatConsole::mMessages;
char ChatConsole::mMessageBuffer[500];

void ChatConsole::Draw()
{
	if (ImGui::Begin("Chat console", &Gui::openedWindows[GUI_WINDOW_ID_CHAT_CONSOLE]))
	{
		if (ImGui::InputText("##message", mMessageBuffer, sizeof(mMessageBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// Send to network
			Globals::gNetwork.client->SendChatMessage(mMessageBuffer);

			AddMessage(Globals::gGame->GetAssignedPlayerID(), mMessageBuffer);
			
			// Clear message
			memset(mMessageBuffer, 0, sizeof(mMessageBuffer));
		}
		for (size_t i = 0; i < mMessages.size(); i++)
			ImGui::Text("%s", mMessages[i].c_str());
	}
	ImGui::End();
}

void ChatConsole::AddMessage(uint32_t playerId, std::string message)
{
	std::string username = Globals::gGame->players[playerId]->GetUsername();

	std::string result = std::string("[").append(username).append("] : ");
	result = result.append(message);

	mMessages.push_back(result);
}
