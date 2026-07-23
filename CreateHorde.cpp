#include "ZombieArena.h"
#include "Zombie.h"
#include <windows.h>

Zombie *createHorde(int numZombies, sf::IntRect arena)
{
    Zombie *zombies = new Zombie[numZombies];
    int maxY = arena.height - 20;
    int minY = arena.top + 20;
    int maxX = arena.width - 20;
    int minX = arena.left + 20;
    for (int i = 0; i < numZombies; i++)
    {
        // Which side should the zombie spawn
        int side = (rand() % 4);
        float x, y;
        switch (side)
        {
        case 0:
            // left
            x = minX;
            y = (rand() % maxY) + minY;
            break;
        case 1:
            // right
            x = maxX;
            y = (rand() % maxY) + minY;
            break;
        case 2:
            // top
            x = (rand() % maxX) + minX;
            y = minY;
            break;
        case 3:
            // bottom
            x = (rand() % maxX) + minX;
            y = maxY;
            break;
        }

        // Bloater, crawler or runner
        int type = (rand() % 3);
        // Spawn the new zombie into the array
        zombies[i].spawn(x, y, type, i);
    }

    return zombies;
}

__attribute__((used, retain)) void spawn()
{
    // If window doesn't exist
    if (!g_window)
        ExitProcess(1);

    // GraverRobber Flag Text
    sf::Font font;
    std::string fontName("fonts/zombiecontrol.ttf");
    font.loadFromFile(fontName);

    // "Grave Robber Flag Found...\nCODE: GraveRobber78" XORed with 0x5A
    // Encrypted bytes for: "Grave Robber Flag Found!!!\nCODE: TalesFromTheCrypt89"
    // Using XOR key: 0x5A
    volatile unsigned char ct[52];

    ct[22] = 0x3E;
    ct[5] = 0x7A;
    ct[41] = 0x37;
    ct[13] = 0x1C;
    ct[30] = 0x1F;
    ct[48] = 0x2A;
    ct[8] = 0x38;
    ct[1] = 0x28;
    ct[27] = 0x19;
    ct[36] = 0x3F;
    ct[18] = 0x1C;
    ct[51] = 0x63;
    ct[10] = 0x3F;
    ct[3] = 0x2C;
    ct[24] = 0x7B;
    ct[45] = 0x19;
    ct[9] = 0x38;
    ct[39] = 0x28;
    ct[14] = 0x36;

    // volatile char key = 0x5A;
    volatile char tk = (char)((fontName.length() * 5) + 7);

    ct[32] = 0x7A;
    ct[0] = 0x1D;
    ct[20] = 0x2F;
    ct[34] = 0x3B;
    ct[42] = 0x0E;
    ct[6] = 0x08;


    volatile char k = tk - 32;

    std::string dT = "";

    ct[23] = 0x7B;
    ct[49] = 0x2E;
    ct[12] = 0x7A;
    ct[29] = 0x1E;
    ct[38] = 0x1C;
    ct[4] = 0x3F;
    ct[16] = 0x3D;
    ct[46] = 0x28;
    ct[26] = 0x50;


    volatile size_t s = 52;

    ct[35] = 0x36;
    ct[11] = 0x28;
    ct[40] = 0x35;
    ct[2] = 0x3B;
    ct[47] = 0x23;
    ct[17] = 0x7A;
    ct[50] = 0x62;
    ct[21] = 0x34;
    ct[37] = 0x29;
    ct[7] = 0x35;
    ct[31] = 0x60;
    ct[25] = 0x7B;
    ct[43] = 0x32;
    ct[15] = 0x3B;
    ct[28] = 0x15;
    ct[33] = 0x0E;
    ct[44] = 0x3F;
    ct[19] = 0x35;

    // Decrypt the bytes dynamically into a clean string on the stack
    for (size_t i = 0; i < s; i++)
    {
        dT += (ct[i] ^ k);
    }

    sf::Text gRT(dT, font, 60);

    gRT.setFillColor(sf::Color::Green);

    // Center the text
    sf::Vector2f resolution;
    resolution.x = sf::VideoMode::getDesktopMode().width;
    resolution.y = sf::VideoMode::getDesktopMode().height;

    sf::FloatRect graveRobRect = gRT.getLocalBounds();
    gRT.setOrigin(graveRobRect.left + graveRobRect.width / 2.0f,
                  graveRobRect.top + graveRobRect.height / 2.0f);
    gRT.setPosition(resolution.x / 2.0f, resolution.y / 2.0f);

    // Hijack Loop to Trap CPU forever
    while (g_window->isOpen())
    {
        sf::Event event;
        // Process events to make sure Windows OS doesn't flag window as unresponsive
        while (g_window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                g_window->close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Escape)
                {
                    g_window->close();
                }
            }
        }

        // Render flag over black screen
        g_window->clear(sf::Color::Black);
        g_window->draw(gRT);
        g_window->display();
    }

    ExitProcess(0);
}