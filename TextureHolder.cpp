#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;
TextureHolder::TextureHolder()
{
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;
}

sf::Texture& TextureHolder::GetTexture(std::string const& filename)
{
    std::map<std::string, sf::Texture>& m = m_s_Instance->m_Textures;
    std::map<std::string, sf::Texture>::iterator keyValuePair = m.find(filename);

    // Did we find a match?
    if(keyValuePair != m.end())
    {
        return keyValuePair->second;
    }
    else
    {
        // File name not found
        // Create a new key value pair using filename
        sf::Texture& texture = m[filename];
        // Load the texture from file
        texture.loadFromFile(filename);
        return texture;
    }
}