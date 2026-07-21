#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

// FOR DEBUGGING ONLY
#include <iostream>

// -Need to create a gamedata folder and a scores.txt file within it for saving high score

// BUGZZZ
// !!! BUG 0: When shooting 1 bullet it seems that it is bullets[5]


// why is it
int main()
{
    // random seed generator
    srand((int)time(0));

    // Here is the instance of TextureHolder
    TextureHolder holder;

    // The game will always be in one of four states
    enum class State
    {
        PAUSED,
        LEVELING_UP,
        GAME_OVER,
        PLAYING,
        FLAG1,
        FLAG2,
        FLAG3
    };

    bool FLAG1_found = false;
    bool FLAG2_found = false;
    bool FLAG3_found = false;

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
        "Zombie Land", sf::Style::Fullscreen);
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
    // Create an instance of the Player class
    Player player;
    // The boundaries of the arena
    sf::IntRect arena;
    // Create the background
    sf::VertexArray background;
    // Load the texture for our background vertex array
    sf::Texture textureBackground = TextureHolder::GetTexture(
        "graphics/background_sheet.png");

    // Prepare for a horde of zombies
    int numZombies;
    int numZombiesAlive;
    Zombie *zombies = nullptr;

    // 100 bullets should do
    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsSpare = 24;
    // BulletsSpare Tracking for #A1
    int prevBulletsInClip = -1;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;
    // When was the fire button last pressed?
    sf::Time lastPressed;

    // Hide the mouse pointer and replace it with crosshair
    window.setMouseCursorVisible(true);
    sf::Sprite spriteCrosshair;
    sf::Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
    spriteCrosshair.setTexture(textureCrosshair);
    spriteCrosshair.setOrigin(25, 25);

    // Create a couple of pickups
    Pickup healthPickup(1);
    Pickup ammoPickup(2);

    // About the game
    int score = 0;
    int hiScore = 0;

    // For the home/game over screen
    sf::Sprite spriteGameOver;
    sf::Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
    spriteGameOver.setTexture(textureGameOver);
    spriteGameOver.setPosition(0, 0);

    // Create a view for the HUD
    // !!! MAKE SURE THAT THE DYNAMIC VERSION WORKS CORRECTLY !!!
    // sf::View hudView(sf::FloatRect(0, 0, 1920, 1080));
    sf::View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
    // Create a sprite for the ammo icon
    sf::Sprite spriteAmmoIcon;
    sf::Texture textureAmmoIcon = TextureHolder::GetTexture(
        "graphics/ammo_icon.png");
    spriteAmmoIcon.setTexture(textureAmmoIcon);
    spriteAmmoIcon.setPosition(20, 980);
    
    // Load the font
    sf::Font font;
    font.loadFromFile("fonts/zombiecontrol.ttf");

    // Paused
    sf::Text pausedText;
    pausedText.setFont(font);
    pausedText.setCharacterSize(50);
    pausedText.setFillColor(Color::White);
    pausedText.setString("Press Enter to continue\n\nW: Move Up\n"
        "S: Move Down\nA: Move Left\n"
        "D: Move Right\nR: Reload\nLeft Mouse Click: Fire");

    sf::FloatRect pausedRect = pausedText.getLocalBounds();
    pausedText.setOrigin(pausedRect.left + pausedRect.width / 2.0f,
                        pausedRect.top + pausedRect.height / 2.0f);
    pausedText.setPosition(resolution.x / 2.0f, resolution.y / 2.0f);


    // FLAG1_Text
    sf::Text Flag1_Text;
    Flag1_Text.setFont(font);
    Flag1_Text.setCharacterSize(50);
    Flag1_Text.setFillColor(Color::Red);
    Flag1_Text.setString("Infinite Ammo Flag Found!!! \n"
        "CODE: 123456789\n\n\nPress \"Enter\" to Continue\n");

    sf::FloatRect Flag1_Rect = Flag1_Text.getLocalBounds();
    Flag1_Text.setOrigin(Flag1_Rect.left + Flag1_Rect.width / 2.0f,
                        Flag1_Rect.top + Flag1_Rect.height / 2.0f);
    Flag1_Text.setPosition(resolution.x / 2.0f, resolution.y / 2.0f);


    // FLAG2_Text

    // FLAG3_Text

    // Game Over
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(125);
    gameOverText.setFillColor(Color::White);
    gameOverText.setString("ZOMBIE LAND \n\nPress Enter to play");
        
    sf::FloatRect gameOverRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(gameOverRect.left + gameOverRect.width / 2.0f, 
                            gameOverRect.top + gameOverRect.height / 2.0f);
    gameOverText.setPosition(resolution.x / 2.0f, resolution.y / 2.0f);

    // LEVELING up
    sf::Text levelUpText;
    levelUpText.setFont(font);
    levelUpText.setCharacterSize(80);
    levelUpText.setFillColor(Color::White);
    std::stringstream levelUpStream;
    levelUpStream << "1- Increased rate of fire" << "\n2- Increased clip size(next reload)" << "\n3- Increased max health" << "\n4- Increased run speed" << "\n5- More and better health pickups" << "\n6- More and better ammo pickups";
    levelUpText.setString(levelUpStream.str());
    
    sf::FloatRect levelUpRect = levelUpText.getLocalBounds();
    levelUpText.setOrigin(levelUpRect.left + levelUpRect.width / 2.0f,
                        levelUpRect.top + levelUpRect.height / 2.0f);
    levelUpText.setPosition(resolution.x / 2.0f, resolution.y / 2.0f);

    // Ammo
    sf::Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(55);
    ammoText.setFillColor(sf::Color::White);

    sf::FloatRect ammoTextRect = ammoText.getLocalBounds();
    ammoText.setOrigin(ammoTextRect.left + ammoTextRect.width / 2.0f,
                        ammoTextRect.top + ammoTextRect.height / 2.0f);
    ammoText.setPosition(20, resolution.y - 200);

    // Score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(55);
    scoreText.setFillColor(sf::Color::White);

    sf::FloatRect scoreTextRect = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreTextRect.left + scoreTextRect.width / 2.0f,
                            scoreTextRect.top + scoreTextRect.height / 2.0f);
    scoreText.setPosition(20,0);

    // Load the high score from text file
    std::ifstream inputFile("gamedata/scores.txt");
    if (inputFile.is_open())
    {
        // >> Reads the data
        inputFile >> hiScore;
        inputFile.close();
    }

    // Hi Score
    sf::Text hiScoreText;
    hiScoreText.setFont(font);
    hiScoreText.setCharacterSize(55);
    hiScoreText.setFillColor(sf::Color::White);
    hiScoreText.setPosition(resolution.x - 300, 0);
    std::stringstream s;
    s << "Hi Score:" << hiScore;
    hiScoreText.setString(s.str());


    // Zombies remaining
    sf::Text zombiesRemainingText;
    zombiesRemainingText.setFont(font);
    zombiesRemainingText.setCharacterSize(55);
    zombiesRemainingText.setFillColor(Color::White);
    zombiesRemainingText.setPosition(resolution.x - 300, resolution.y-300);
    zombiesRemainingText.setString("Zombies: 100");


    // Wave number
    int wave = 0;
    sf::Text waveNumberText;
    waveNumberText.setFont(font);
    waveNumberText.setCharacterSize(55);
    waveNumberText.setFillColor(sf::Color::White);
    waveNumberText.setPosition(resolution.x - 300, resolution.y - 200);
    waveNumberText.setString("Wave: 0");

    // Health bar
    sf::RectangleShape healthBar;
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(450, resolution.y - 100);

    // When did we last update the HUD?
    int framesSinceLastHUDUpdate = 0;
    // How often (in frames) should we update the HUD
    int fpsMeasurementFrameInterval = 1000;

    sf::SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("sound/hit.wav");
    sf::Sound hit;
    hit.setBuffer(hitBuffer);
    // Prepare the splat sound
    sf::SoundBuffer splatBuffer;
    splatBuffer.loadFromFile("sound/splat.wav");
    sf::Sound splat;
    splat.setBuffer(splatBuffer);
    // Prepare the shoot sound
    sf::SoundBuffer shootBuffer;
    shootBuffer.loadFromFile("sound/shoot.wav");
    sf::Sound shoot;
    shoot.setBuffer(shootBuffer);
    // Prepare the reload sound
    sf::SoundBuffer reloadBuffer;
    reloadBuffer.loadFromFile("sound/reload.wav");
    sf::Sound reload;
    reload.setBuffer(reloadBuffer);
    // Prepare the failed sound
    sf::SoundBuffer reloadFailedBuffer;
    reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
    sf::Sound reloadFailed;
    reloadFailed.setBuffer(reloadFailedBuffer);
    // Prepare the powerup sound
    sf::SoundBuffer powerupBuffer;
    powerupBuffer.loadFromFile("sound/powerup.wav");
    sf::Sound powerup;
    powerup.setBuffer(powerupBuffer);
    // Prepare the pickup sound
    sf::SoundBuffer pickupBuffer;
    pickupBuffer.loadFromFile("sound/pickup.wav");
    sf::Sound pickup;
    pickup.setBuffer(pickupBuffer);

    // The main game loop
    while (window.isOpen())
    {
        /*
    ************
    Handle input
    ************
    */
        // Handle events by polling
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                // Pause a game while playing
                if (event.key.code == sf::Keyboard::Return &&
                    state == State::PLAYING)
                {
                    state = State::PAUSED;
                }
                // Restart while paused
                else if (event.key.code == sf::Keyboard::Return &&
                         (state == State::PAUSED ||
                          state == State::FLAG1 ||
                          state == State::FLAG2 ||
                          state == State::FLAG3
                        ))
                {
                    state = State::PLAYING;
                    // Reset the clock so there isn't a frame jump
                    clock.restart();
                }
                // Start a new game while in GAME_OVER state
                else if (event.key.code == sf::Keyboard::Return &&
                         state == State::GAME_OVER)
                {
                    state = State::LEVELING_UP;
                    wave = 0;
                    score = 0;
                    // Prepare the gun and ammo for next game
                    currentBullet = 0;
                    bulletsSpare = 24;
                    bulletsInClip = 6;
                    clipSize = 6;
                    fireRate = 1;
                    // Reset the player's stats
                    player.resetPlayerStats();
                }
                if (state == State::PLAYING)
                {
                    // Reloading
                    if (event.key.code == sf::Keyboard::R)
                    {
                        if (bulletsSpare >= clipSize && bulletsInClip == 0)
                        {
                            // Plenty of bullets. Reload.
                            bulletsInClip = clipSize;
                            bulletsSpare -= clipSize;
                            reload.play();
                        }
                        else if (bulletsSpare > 0 && bulletsInClip == 0)
                        {
                            // Only few bullets left
                            bulletsInClip = bulletsSpare;
                            bulletsSpare = 0;
                            reload.play();
                        }
                        else if (bulletsInClip == 0)
                        {
                            reloadFailed.play();
                        }
                        else
                        {
                            // No sound
                        }
                    }
                }
            }
        } // End event polling

        // Handle the player quitting
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        // Handle WASD while playing
        if (state == State::PLAYING)
        {
            // Handle the pressing and releasing of WASD keys
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                player.moveUp();
            }
            else
            {
                player.stopUp();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                player.moveDown();
            }
            else
            {
                player.stopDown();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                player.moveLeft();
            }
            else
            {
                player.stopLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                player.moveRight();
            }
            else
            {
                player.stopRight();
            }

            // Fire a bullet
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
                {
                    // Pass the centre of the player
                    // and the centre of the cross-hair
                    // to the shoot function
                    bullets[currentBullet].shoot(
                        player.getCenter().x, player.getCenter().y,
                        mouseWorldPosition.x, mouseWorldPosition.y);
                    currentBullet++;
                    if (currentBullet > 99)
                    {
                        currentBullet = 0;
                    }
                    lastPressed = gameTimeTotal;
                    shoot.play();
                    std::cout << "BulletsInClipB4: " << bulletsInClip << std::endl;
                    std::cout << "PreviousB4: " << prevBulletsInClip << std::endl;
                    bulletsInClip--;
                    
                    if(bulletsInClip == prevBulletsInClip)
                        {
                            //CHECK FOR WHAT HAPPENS AT 0 and incorporate
                            std::cout << "Infinite ammo!!!\n";
                            if(FLAG1_found == false){ 
                                state = State::FLAG1;
                                FLAG1_found = true;
                            } 
                        }

                    prevBulletsInClip = bulletsInClip;
                   // std::cout << "BulletsInClipAfter: " << bulletsInClip << std::endl;
                   // std::cout << "Previous(after): " << bulletsInClip << std::endl;
                    
                    
                }
            } // End fire a bullet

        } // End WASD while playing

        // Handle the LEVELING up state
        if (state == State::LEVELING_UP)
        {
            // Handle the player LEVELING up
            if (event.key.code == sf::Keyboard::Num1)
            {
                // Increase fire rate
                fireRate++;
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num2)
            {
                // Increase clip size
                clipSize += clipSize;
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num3)
            {
                // Increase health
                player.upgradeHealth();
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num4)
            {
                // Increase speed
                player.upgradeSpeed();
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num5)
            {
                // Upgrade health pickup
                healthPickup.upgrade();
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num6)
            {
                // Upgrade ammo pickup
                ammoPickup.upgrade();
                state = State::PLAYING;
            }

            if (state == State::PLAYING)
            {
                // Increase the wave number
                wave++;
                // Prepare the level
                // We will modify the next two lines later
                arena.width = 500 * wave;
                arena.height = 500 * wave;
                arena.left = 0;
                arena.top = 0;
                // Pass the vertex array by reference
                // to the createBackground function
                int tileSize = createBackground(background, arena);
                // We will modify this line of code later
                // int tileSize = 50;

                // Spawn the player in middle of the arena
                player.spawn(arena, resolution, tileSize);
                // Create a horde of zombies
                // Configure the pick-ups
                healthPickup.setArena(arena);
                ammoPickup.setArena(arena);

                numZombies = 5 * wave;
                // Delete the previously allocated memory (if it exists)
                delete[] zombies;
                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;
                // Play the powerup sound
                powerup.play();

                // Reset clock so there isn't a frame jump
                clock.restart();
            }
        } // End Leveling Up

        /*
    ****************
    UPDATE THE FRAME
    ****************
    */
        if (state == State::PLAYING)
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

            // Set the crosshair to the mouse world location
            spriteCrosshair.setPosition(mouseWorldPosition);

            // Update the player
            player.update(dtAsSeconds, sf::Mouse::getPosition());
            // Make a note of the players new position
            sf::Vector2f playerPosition(player.getCenter());

            // Make the view centre
            // the around player
            mainView.setCenter(player.getCenter());
            // Loop through each Zombie and update them
            for (int i = 0; i < numZombies; i++)
            {
                if (zombies[i].isAlive())
                {
                    zombies[i].update(dt.asSeconds(), playerPosition);
                }
            }
            // Update any bullets that are in-flight
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    bullets[i].update(dtAsSeconds);
                }
            }

            // Update the pickups
            healthPickup.update(dtAsSeconds);
            ammoPickup.update(dtAsSeconds);

            // Collision detection
            // Have any zombies been shot?
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < numZombies; j++)
                {
                    if (bullets[i].isInFlight() &&
                        zombies[j].isAlive())
                    {
                        if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
                        {
                            // Stop the bullet
                            bullets[i].stop();
                            // Register the hit and see if it was a kill
                            if (zombies[j].hit())
                            {
                                // Not just a hit but a kill too
                                score += 10;
                                if (score >= hiScore)
                                {
                                    hiScore = score;
                                }
                                numZombiesAlive--;
                                // When all the zombies are dead (again)
                                if (numZombiesAlive == 0)
                                {
                                    state = State::LEVELING_UP;
                                }
                                
                                // Make a splat sound
                                splat.play();
                            }
                        }
                    }
                }
            } // End zombie being shot

            // Have any zombies touched the player
            for (int i = 0; i < numZombies; i++)
            {
                if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
                {
                    if (player.hit(gameTimeTotal))
                    {
                        hit.play();
                    }
                    if (player.getHealth() <= 0)
                    {
                        state = State::GAME_OVER;
                        std::ofstream outputFile("gamedata/scores.txt");
                        // write data
                        outputFile << hiScore;
                        outputFile.close();
                    }
                }
            } // End player touched

            // Has the player touched health pickup
            if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
            {
                player.increaseHealthLevel(healthPickup.gotIt());
                // Play a sound
                pickup.play();
            }
            // Has the player touched ammo pickup
            if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
            {
                bulletsSpare += ammoPickup.gotIt();
                // Play a sound
                reload.play();
            }
            // size up the health bar
            healthBar.setSize(sf::Vector2f(player.getHealth() * 3, 50));
            // Increment the number of frames since the previous update
            framesSinceLastHUDUpdate++;
            // re-calculate every fpsMeasurementFrameInterval frames
            if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
            {
                // Update game HUD text
                std::stringstream ssAmmo;
                std::stringstream ssScore;
                std::stringstream ssHiScore;
                std::stringstream ssWave;
                std::stringstream ssZombiesAlive;
                // Update the ammo text
                ssAmmo << bulletsInClip << "/" << bulletsSpare;
                ammoText.setString(ssAmmo.str());
                // Update the score text
                ssScore << "Score:" << score;
                scoreText.setString(ssScore.str());
                // Update the high score text
                ssHiScore << "Hi Score:" << hiScore;
                hiScoreText.setString(ssHiScore.str());
                // Update the wave
                ssWave << "Wave:" << wave;
                waveNumberText.setString(ssWave.str());
                // Update the high score text
                ssZombiesAlive << "Zombies:" << numZombiesAlive;
                zombiesRemainingText.setString(ssZombiesAlive.str());

                framesSinceLastHUDUpdate = 0;
            } // End HUD update

        } // End updating the scene

        /*
        **************
        Draw the scene
        **************
        */
        if (state == State::PLAYING)
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

            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    window.draw(bullets[i].getShape());
                }
            }

            // Draw the pick-ups, if currently spawned
            if (ammoPickup.isSpawned())
            {
                window.draw(ammoPickup.getSprite());
            }

            if (healthPickup.isSpawned())
            {
                window.draw(healthPickup.getSprite());
            }

            // Draw the crosshair
            window.draw(spriteCrosshair);

            // Draw the player
            window.draw(player.getSprite());

            // Switch to the HUD view
            window.setView(hudView);
            // Draw all the HUD elements
            window.draw(spriteAmmoIcon);
            window.draw(ammoText);
            window.draw(scoreText);
            window.draw(hiScoreText);
            window.draw(healthBar);
            window.draw(waveNumberText);
            window.draw(zombiesRemainingText);
        }

        if (state == State::LEVELING_UP)
        {
            window.setView(hudView);
            window.draw(spriteGameOver);
            window.draw(levelUpText);
        }

        if (state == State::PAUSED)
        {
            window.setView(hudView);
            window.draw(pausedText);
        }

        if (state == State::FLAG1)
        {
            window.setView(hudView);
            window.draw(Flag1_Text);
        }

        if (state == State::GAME_OVER)
        {
            window.setView(hudView);
            window.draw(spriteGameOver);
            window.draw(gameOverText);
            window.draw(scoreText);
            window.draw(hiScoreText);
        }

        window.display();

    } // End game loop

    // Delete the previously allocated memory (if it exists)
    delete[] zombies;

    return 0;
}