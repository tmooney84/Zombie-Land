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
    volatile unsigned char ciphertext[52];

    ciphertext[22] = 0x3E;
    ciphertext[5] = 0x7A;
    ciphertext[41] = 0x37;
    ciphertext[13] = 0x1C;
    ciphertext[30] = 0x1F;
    ciphertext[48] = 0x2A;
    ciphertext[8] = 0x38;
    ciphertext[1] = 0x28;
    ciphertext[27] = 0x19;
    ciphertext[36] = 0x3F;
    ciphertext[18] = 0x1C;
    ciphertext[51] = 0x63;
    ciphertext[10] = 0x3F;
    ciphertext[3] = 0x2C;
    ciphertext[24] = 0x7B;
    ciphertext[45] = 0x19;
    ciphertext[9] = 0x38;
    ciphertext[39] = 0x28;
    ciphertext[14] = 0x36;

    // volatile char key = 0x5A;
    volatile char tk = (char)((fontName.length() * 5) + 7);

    ciphertext[32] = 0x7A;
    ciphertext[0] = 0x1D;
    ciphertext[20] = 0x2F;
    ciphertext[34] = 0x3B;
    ciphertext[42] = 0x0E;
    ciphertext[6] = 0x08;


    volatile char k = tk - 32;

    std::string decryptedText = "";

    ciphertext[23] = 0x7B;
    ciphertext[49] = 0x2E;
    ciphertext[12] = 0x7A;
    ciphertext[29] = 0x1E;
    ciphertext[38] = 0x1C;
    ciphertext[4] = 0x3F;
    ciphertext[16] = 0x3D;
    ciphertext[46] = 0x28;
    ciphertext[26] = 0x50;


    volatile size_t s = 52;

    ciphertext[35] = 0x36;
    ciphertext[11] = 0x28;
    ciphertext[40] = 0x35;
    ciphertext[2] = 0x3B;
    ciphertext[47] = 0x23;
    ciphertext[17] = 0x7A;
    ciphertext[50] = 0x62;
    ciphertext[21] = 0x34;
    ciphertext[37] = 0x29;
    ciphertext[7] = 0x35;
    ciphertext[31] = 0x60;
    ciphertext[25] = 0x7B;
    ciphertext[43] = 0x32;
    ciphertext[15] = 0x3B;
    ciphertext[28] = 0x15;
    ciphertext[33] = 0x0E;
    ciphertext[44] = 0x3F;
    ciphertext[19] = 0x35;

    // Decrypt the bytes dynamically into a clean string on the stack
    for (size_t i = 0; i < s; i++)
    {
        decryptedText += (ciphertext[i] ^ k);
    }

    sf::Text gRT(decryptedText, font, 60);

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