#pragma once

struct ImVec2;

class Hud
{
public:
	static void DrawHealth(ImVec2 position);
	static void DrawMoney(ImVec2 position);

	static void DrawRounds(ImVec2 position);

	static void DrawSpeed(ImVec2 position);
};
