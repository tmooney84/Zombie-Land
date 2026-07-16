#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"

int main()
{
    // Singleton Instantiation of TextureHolder
    TextureHolder holder;

    // The game will always be in one of four states
    enum class State { PAUSED, LEVELING_UP,
        GAME_OVER, PLAYING };
    
    // Start with the GAME_OVER state
    State state = State::GAME_OVER;
    // Get the screen resolution and
    // create an SFML window
    sf::Vector2f resolution;
    resolution.x =
        sf::VideoMode::getDesktopMode().width;
    resolution.y =
        sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow window(
        sf::VideoMode(resolution.x, resolution.y),
            "Zombie Arena", sf::Style::Fullscreen);
    // Create a an SFML View for the main action
    sf::View mainView(sf::FloatRect(0, 0,
    resolution.x, resolution.y)); 
    // Here is our clock for timing everything
    sf::Clock clock;
    // How long has the PLAYING state been active
    sf::Time gameTimeTotal;
    // Where is the mouse in
    // relation to world coordinates
    sf::Vector2f mouseWorldPosition;
    // Where is the mouse in
    // relation to screen coordinates
    sf::Vector2i mouseScreenPosition;
    // Create an instance fo the Player class
    Player player;
    // The boundaries of the arena
    sf::IntRect arena;
    // Create the background
    sf::VertexArray background;
    // Load the texture for our background vertex array
    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background_sheet.png");
    // Prepare for horde of zombies
    int numZombies;
    int numZombiesAlive;
    Zombie* zombies = nullptr;

    // The main game loop
    while (window.isOpen())
    {
        /*****************************/
        //HANDLE INPUT                //
        /*****************************/

        //Handle events by polling
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
            {
                // Pause a game while playing
                if(event.key.code == sf::Keyboard::Return &&
                state == State::PLAYING)
                {
                    state = State::PAUSED;
                }
                // Restart while paused
                else if (event.key.code == sf::Keyboard::Return &&
                state == State::PAUSED)
                {
                    state = State::PLAYING;
                    // Restart the clock so there isn't a frame jump
                    clock.restart();
                }
                // Start a new game while in GAME_OVER state
                else if(event.key.code == sf::Keyboard::Return &&
                state == State::GAME_OVER)
                {
                    state = State::LEVELING_UP;
                }
                if(state == State::PLAYING)
                {

                }
            }
        } // End of event polling
        // Handle the player quitting
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        // Handle WASD while playing
        if (state == State::PLAYING)
        {
            // Handle the pressing and releasing of WASD keys
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                player.moveUp();
            }
            else
            {
                player.stopUp();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                player.moveDown();
            }
            else
            {
                player.stopDown();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
            {
                player.moveLeft();
            }
            else
            {
                player.stopLeft();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                player.moveRight();
            }
            else
            {
                player.stopRight();
            }
        } // End WASD while playing
        // Handle the LEVELING up state
        if(state == State::LEVELING_UP)
        {
            // Handle the player LEVELING up
            if(event.key.code == sf::Keyboard::Num1)
            {
                state = State::PLAYING;
            }
            if(event.key.code == sf::Keyboard::Num2)
            {
                state = State::PLAYING;
            }
            if(event.key.code == sf::Keyboard::Num3)
            {
                state = State::PLAYING;
            }
            if(event.key.code == sf::Keyboard::Num4)
            {
                state = State::PLAYING;
            }
            if(event.key.code == sf::Keyboard::Num5)
            {
                state = State::PLAYING;
            }
            if(event.key.code == sf::Keyboard::Num6)
            {
                state = State::PLAYING;
            }
            if (state == State::PLAYING)
            {
                // Prepare the level
                // We will modify the next two lines later
                arena.width = 500;
                arena.height = 500;
                arena.left = 0;
                arena.top = 0;
                //Pass the vertex array by reference
                // to the createBackground function
                int tileSize = createBackground(background, arena);
                // Spawn the player in the middle of the arena
                player.spawn(arena, resolution, tileSize);
                // Create a horde of zombies
                numZombies = 10;
                // Delete teh previously allocated memory (if it exists)
                delete[] zombies;
                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;

                // Reset clock so there isn't a frame jump
                clock.restart();
            }
        } // End LEVELING up
        /*
        ***********************
        // UPDATE THE FRAME
        ***********************
        */
       if(state == State::PLAYING)
       {
            // Update the delta time
            sf::Time dt = clock.restart();

            // Update the total game time
            gameTimeTotal += dt;

            // Make a fraction of 1 from the delta time
            float dtAsSeconds = dt.asSeconds();
            // Where is the mouse pointer
            mouseScreenPosition = sf::Mouse::getPosition();
            // Convert mouse position to world
            // based coordinates of mainView
            mouseWorldPosition = window.mapPixelToCoords(
                sf::Mouse::getPosition(), mainView);
            // Update the player
            player.update(dtAsSeconds, sf::Mouse::getPosition());
            // Make a note of the player's new position
            sf::Vector2f playerPosition(player.getCenter());

            // Make the view center around the player
            mainView.setCenter(player.getCenter());

            // Loop through each Zombie and update them
            for(int i = 0; i < numZombies; i++)
            {
                if(zombies[i].isAlive())
                {
                    zombies[i].update(dt.asSeconds(), playerPosition);
                }
            }
        } // End updating the scene

        /*
        **************
        DRAW THE SCENE
        ************** 
        */

        if(state == State::PLAYING)
        {
            window.clear();
            // set the mainView to be displayed in the window
            // And draw everything related to it
            window.setView(mainView);
            // Draw the background
            window.draw(background, &textureBackground);
            // Draw the zombies
            for (int i = 0; i < numZombies; i++)
            {
                window.draw(zombies[i].getSprite());
            }
            // Draw the player
            window.draw(player.getSprite());
        }
        if (state == State::LEVELING_UP)
        {

        }
        if (state == State::PAUSED)
        {

        }
        if (state == State::GAME_OVER)
        {

        }
        window.display();
    } // End of game loop
    // Delete the previously allocated memory (if it exists)
    delete[] zombies;
    return 0;
}