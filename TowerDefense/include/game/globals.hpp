#pragma once

#include <Vector2.h>
#include <imgui.h>
#include <stdint.h>
#include "discordRPC/discord_rpc.h"
#include "network/client_interface.hpp"
#include "network/server_interface.hpp"

#include "game.hpp"
#include "resources.hpp"

class Globals
{
public:
	static ImDrawList* gDrawList;
	static ImGuiIO* gIO;
	
	static ImFont* gFontBig;
	static ImFont* gFontSemiBig;
	static ImFont* gFontMedium;

	static Game* gGame;
	static Resources* gResources;
	static DiscordRichPresence gDiscordRpc;

	static NetworkClient gClient;
	static NetworkServer* gServer;

	static int32_t gWindowX;
	static int32_t gWindowY;
	static int32_t gWindowWidth;
	static int32_t gWindowHeight;

	static int32_t gGridX;
	static int32_t gGridY;

	static bool fullscreen;

	static void UpdateGlobals();
	static void InitGlobals(Game* g);
	static void DestroyGlobals();
};
