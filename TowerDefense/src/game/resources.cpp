#include "resources.hpp"

#include <filesystem>
#include <iostream>

Resources::Resources()
{
    std::filesystem::path assetsPath = std::filesystem::current_path().append("assets");
    for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(assetsPath))
    {
        if (dirEntry.is_directory() || dirEntry.path().extension().string() != ".png")
            continue;

        std::cout << "Loading '" << std::filesystem::relative(dirEntry.path(), assetsPath).string() << "'...";
        Load(std::filesystem::relative(dirEntry.path(), assetsPath).string());
    }
}

Resources::~Resources()
{
    for (std::map<std::string, Texture*>::iterator _item = mTextures.begin(); _item != mTextures.end(); _item++)
    {
        Texture* tex = _item->second;

        ImGuiUtils::UnloadTexture(tex);
        delete tex;
    }
}

Texture *Resources::GetTexture(std::string name)
{
    Texture* tex = mTextures[name + ".png"];
    assert(tex != nullptr && ("A non-existing texture was requested: " + name).c_str());
    return tex;
}

void Resources::Load(std::string name, bool nearestFilter)
{
    Texture* texture = ImGuiUtils::LoadTexture(("assets\\" + name).c_str(), nearestFilter);
    if (texture)
    {
        mTextures.emplace(name, texture);
        std::cout << " Success!" << std::endl;
    }
    else
    {
        std::cerr << " Error!"<< std::endl;
    }
}
