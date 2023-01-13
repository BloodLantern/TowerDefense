#pragma once

#include "imgui_utils.hpp"

#include <string>
#include <map>

class Resources
{
public:
    Resources();
    ~Resources();
    
    Texture* GetTexture(std::string name);

private:
    void Load(std::string name, bool nearestFilter = true);

    std::map<std::string, Texture*> mTextures;
};