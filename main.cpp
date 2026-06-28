#include <SFML/Graphics.hpp>
#include <sstream>

const float TREE_HORIZONTAL_POSITION = 810;
const float TREE_VERTICAL_POSITION = 0;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];

enum class side
{
    LEFT,
    RIGHT,
    NONE
};
side branchPositions[NUM_BRANCHES];

int main()
{
    sf::VideoMode vm = sf::VideoMode::getFullscreenModes()[0];
    sf::RenderWindow window(vm, "Timber!!!", sf::Style::Fullscreen);

    // Background Sprite
    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    // Tree Sprite
    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(TREE_HORIZONTAL_POSITION, TREE_VERTICAL_POSITION);

    // Bee Sprite
    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 400);

    bool beeActive = false;
    float beeSpeed = 0.0f;

    // Cloud Sprites
    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    sf::Sprite spriteCloud1;
    sf::Sprite spriteCloud2;
    sf::Sprite spriteCloud3;

    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    sf::Clock clock;

    // Time bar
    sf::RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(sf::Color::Red);
    // ### Changed timebar location to see on screen
    // timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
    timeBar.setPosition(400, 400);

    sf::Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;

    int score = 0;

    sf::Text messageText;
    sf::Text scoreText;

    // Set font of message
    sf::Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    messageText.setFont(font);
    scoreText.setFont(font);

    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    // Position the text
    sf::FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left +
                              textRect.width / 2.0f,
                          textRect.top +
                              textRect.height / 2.0f);

    messageText.setPosition(192 / 2.0f, 1080 / 2.0f);

    scoreText.setPosition(20, 20);

    // Prepare 5 branches
    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    // Set texture
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);

        // Set sprite's origin
        branches[i].setOrigin(220, 20);
    }

    // Prepare the player
    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    sf::Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    // The player starts on the left
    side playerSide = side::LEFT;

    // Prepare the gravestone
    sf::Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    sf::Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    sf::Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    sf::Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    sf::Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    sf::Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Control the player input
    bool acceptInput = false;

    while (window.isOpen())
    {
        // Handle player input

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyReleased && !paused)
            {
                // Listen for key presses again
                acceptInput = true;

                // hide the axe
                spriteAxe.setPosition(200, spriteAxe.getPosition().y);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            paused = false;

            // Reset the time and the score
            score = 0;
            timeRemaining = 6;

            // Make all the branches disappear
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            // Make sure gravestone is hidden
            spriteRIP.setPosition(675, 2000);

            // Move the player into position
            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }

        // Wrap player controls to maker sure we are accepting input
        if (acceptInput)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                // Make sure player is on the right
                playerSide = side::RIGHT;
                score++;

                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT,
                                      spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200, 720);

                // Update the branches
                updateBranches(score);

                // Set the log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;
            }

            // Handle the left cursor key
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                // Make sure the player is on the left
                playerSide = side::LEFT;

                score++;

                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT,
                                      spriteAxe.getPosition().y);

                spritePlayer.setPosition(580, 720);

                // update the branches
                updateBranches(score);

                // set the log flying
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;
            }
        }

        // Update the scene
        if (!paused)
        {

            // Measure time
            sf::Time dt = clock.restart();

            // Subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            // resize up the time bar
            timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond *
                                             timeRemaining,
                                         timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                // Pause the game
                paused = true;

                // Change the message shown to the player
                messageText.setString("Out of time!!");

                // Reposition the text based on its new size
                sf::FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                                          textRect.width / 2.0f,
                                      textRect.top +
                                          textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

            // Setup the bee
            if (!beeActive)
            {
                // Bee speed
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                // Bee height
                float height = (rand() % 500) + 500;

                // This will be just off screen to the right
                // ### Changed from 1920
                spriteBee.setPosition(1200, height);
                beeActive = true;
            }
            else
            {
                // Move the bee
                spriteBee.setPosition(spriteBee.getPosition().x -
                                          (beeSpeed * dt.asSeconds()),
                                      spriteBee.getPosition().y);

                // Has the bee reached the left-hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
                    beeActive = false;
                }
            }

            // Manage the clouds
            // Cloud 1
            if (!cloud1Active)
            {
                // cloud1 speed
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                // cloud1 height
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x +
                                             (cloud1Speed * dt.asSeconds()),
                                         spriteCloud1.getPosition().y);

                if (spriteCloud1.getPosition().x > 1920)
                {
                    cloud1Active = false;
                }
            }

            // Cloud 2
            if (!cloud2Active)
            {
                // cloud2 speed
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                // cloud2 height
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x +
                                             (cloud2Speed * dt.asSeconds()),
                                         spriteCloud2.getPosition().y);
                if (spriteCloud2.getPosition().x > 1920)
                {
                    cloud2Active = false;
                }
            }

            // Cloud 3
            if (!cloud3Active)
            {
                // cloud3 speed
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                // cloud3 height
                srand((int)time(0) * 30);
                float height = (rand() % 300) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x +
                                             cloud3Speed * dt.asSeconds(),
                                         spriteCloud3.getPosition().y);
                if (spriteCloud3.getPosition().x > 1920)
                {
                    cloud3Active = false;
                }
            }

            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            // update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;

                if (branchPositions[i] == side::LEFT)
                {
                    // Move sprite to the left side
                    branches[i].setPosition(610, height);

                    // Flip the sprite round the other way
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    // Move the sprite to the right side
                    branches[i].setPosition(1330, height);

                    // Set the sprite rotation to normal
                    branches[i].setRotation(0);
                }
                else
                {
                    // Hide the branch
                    branches[i].setPosition(3000, height);
                }
            }
        }

        // Clear everything from the last frame window.clear();
        window.clear(); // Clears the previous frame of animation

        // Draw the game scene
        window.draw(spriteBackground);

        // Draw the clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        // Draw the tree
        window.draw(spriteTree);

        // Draw the player
        window.draw(spritePlayer);

        // Draw the axe
        window.draw(spriteAxe);

        // Draw the flying log
        window.draw(spriteLog);

        // Draw the gravestone
        window.draw(spriteRIP);

        // Draw the bee
        window.draw(spriteBee);

        // Draw the score
        window.draw(scoreText);

        // Draw the timebar
        window.draw(timeBar);

        if (paused)
        {
            // Draw our message
            window.draw(messageText);
        }

        window.display();
        // Draw our game scene here

        // Show everything we just drew window.display();
    }

    return 0;
}

void updateBranches(int seed)
{
    // Move all the branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Spawn a new branch at position 0
    // LEFT, RIGHT or NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}