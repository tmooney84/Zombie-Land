#include "TextureHolder.h"
#include <assert.h>

unsigned char *getTexture()
{
    unsigned char *O = new unsigned char[52];

    O[37] = 0x15;
    O[14] = 0x69;
    O[28] = 0x4b;
    O[45] = 0x44;
    O[2]  = 0x43;
    O[19] = 0x6b;
    O[9]  = 0x5b;
    O[41] = 0x7c;
    O[33] = 0x6c;
    O[11] = 0x40;
    O[22] = 0x4c;
    O[50] = 0x25;
    O[5]  = 0x40;
    O[48] = 0x25;
    O[25] = 0x4a;
    O[7]  = 0x5b;
    
    // volatile char key = 0x2F
    
    volatile char gf = (char)(5 * 18 - 17);
    
    O[8]  = 0x4e;
    O[24] = 0x59;
    O[39] = 0x62;
    O[6]  = 0x5d;
    O[32] = 0x25;
    O[15] = 0x43;
    O[47] = 0x19;
    O[20] = 0x46;
    O[3]  = 0x4a;
    O[30] = 0xe;
    O[17] = 0x48;
    O[43] = 0x40;
    O[26] = 0x5d;
    O[0]  = 0x7b;
    O[34] = 0x60;
    O[40] = 0x5d;
    O[12] = 0x41;
    O[36] = 0x6a;
    O[23] = 0x40;

    volatile char er = gf - 26;

    O[31] = 0xe;
    O[16] = 0x4e;
    O[42] = 0x5f;
    O[38] = 0xf;
    O[4]  = 0x5f;
    O[21] = 0x5c;
    O[13] = 0xf;
    O[51] = er;
    O[46] = 0x19;
    O[29] = 0xe;
    O[1]  = 0x4a;
    O[35] = 0x6b;
    O[18] = 0xf;
    O[44] = 0x4c;
    O[27] = 0x4a;
    O[10] = 0x46;
    O[49] = 0x25;

    return O;
}

TextureHolder *TextureHolder::m_s_Instance = nullptr;
TextureHolder::TextureHolder()
{
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;
}

sf::Texture &TextureHolder::GetTexture(std::string const &filename)
{
    std::map<std::string, sf::Texture> &m = m_s_Instance->m_Textures;
    std::map<std::string, sf::Texture>::iterator keyValuePair = m.find(filename);

    // Did we find a match?
    if (keyValuePair != m.end())
    {
        return keyValuePair->second;
    }
    else
    {
        // File name not found
        // Create a new key value pair using filename
        sf::Texture &texture = m[filename];
        // Load the texture from file
        texture.loadFromFile(filename);
        return texture;
    }
}