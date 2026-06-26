#include <SFML/Graphics.hpp>
#include <sstream>

const float TREE_HORIZONTAL_POSITION = 810;
const float TREE_VERTICAL_POSITION = 0;

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
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

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

    while (window.isOpen())
    {
        // Handle player input

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
                timeRemaining, timeBarHeight)); 
           
            if(timeRemaining <= 0.0f){
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
                spriteBee.setPosition(1920, height);
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
        }

        // Clear everything from the last frame window.clear();
        window.clear(); // Clears the previous frame of animation

        // Draw the game scene
        window.draw(spriteBackground);

        // Draw the clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw the tree
        window.draw(spriteTree);

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