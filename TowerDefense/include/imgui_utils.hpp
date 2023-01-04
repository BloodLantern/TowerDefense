#pragma once

#include <imgui.h>

struct Texture
{
    ImTextureID id;
    int width;
    int height;
};

class ImGuiUtils
{
public:
    static Texture LoadTexture(const char* file);
    static void UnloadTexture(const Texture& texture);

    static void DrawTextureEx(ImDrawList& dl, const Texture& tex, ImVec2 pos, ImVec2 scale = { 1.f, 1.f }, float angle = 0.f);
};
