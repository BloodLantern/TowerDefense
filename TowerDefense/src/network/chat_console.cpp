#include "network/chat_console.hpp"
#include "imgui.h"
#include "globals.hpp"

std::vector<std::string> ChatConsole::mMessages;
char ChatConsole::mMessageBuffer[500];

void ChatConsole::Draw()
{
	if (ImGui::Begin("Chat console"))
	{
		if (ImGui::InputText("##message", mMessageBuffer, sizeof(mMessageBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			Globals::gClient.SendChatMessage(mMessageBuffer);
		}
		for (size_t i = 0; i < mMessages.size(); i++)
			ImGui::Text("%s", mMessages[i].c_str());
	}
	ImGui::End();
}

void ChatConsole::AddMessage(std::string message)
{
	mMessages.push_back(message);
}
