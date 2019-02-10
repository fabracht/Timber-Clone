
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <sstream>
#include <ctime>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

constexpr int windowWidth{800}, windowHeight{600};
constexpr float timerWidth{200}, timerHeight{50};
constexpr int numberofBranches{6};

enum branchSide {
    LEFT,
    RIGHT,
    NONE,
};

struct branches {

    sf::Texture branch;
//    sf::Texture log;
    
    sf::Sprite branchSprite[numberofBranches];

    branches(float scaleX, float scaleY) {
        if (!branch.loadFromFile(resourcePath() + "graphics/branch.png")) {
            return EXIT_FAILURE;
        }
//        if (!log.loadFromFile(resourcePath() + "graphics/log.png")) {
//            return EXIT_FAILURE;
//        }
        unsigned branchWidth, branchHeight, logWidth, logHeight;
        
        for (size_t i = 0; i < numberofBranches; ++i) {
            int hasBranch = static_cast<int>(rand()%3);
            branchSprite[i].setScale(scaleX, scaleY);
            branchWidth = branch.getSize().x;
            branchHeight = branch.getSize().y;
//            logWidth = log.getSize().x;
//            logHeight = log.getSize().y;
            
            switch(hasBranch) {
                case LEFT:
                    puts("This is a Left");
                    branchSprite[i].setTexture(branch);
//                    branchSprite[i].setScale(scaleX, scaleY);
                    branchSprite[i].setRotation(180.f);
                    branchSprite[i].setOrigin(branchWidth/2.f, branchHeight/2.f);
                    branchSprite[i].setPosition(sf::Vector2f(windowWidth/2.f - branchWidth/2.9f, branchHeight*i));
                    break;
                case RIGHT:
                    puts("This is a Right");
                    branchSprite[i].setTexture(branch);
//                    branchSprite[i].setScale(scaleX, scaleY);
                    branchSprite[i].setOrigin(branchWidth/2.f, branchHeight/2.f);
                    branchSprite[i].setPosition(sf::Vector2f(windowWidth/2.f + branchWidth/2.9f, branchHeight*i));
                    break;
                case NONE:
//                    puts("This is a Log");
//                    branchSprite[i].setTexture(log);
////                    branchSprite[i].setScale(scaleX, scaleY);
//                    branchSprite[i].setOrigin(logWidth/2.f, logHeight/2.f);
//                    branchSprite[i].setPosition(sf::Vector2f(windowWidth/2.f, blockHeight*i));
                    continue;
                default:
                    break;
            }

        }
    }
    
//    float y() { return branchSprite[0].getPosition().y;}
};

struct timerBar {
    
    sf::RectangleShape shape;
    
    timerBar(float mX, float mY) {
        shape.setPosition((windowWidth - timerWidth)/2.f, (windowHeight - timerHeight));
        shape.setSize(sf::Vector2f(mX, mY));
        shape.setFillColor(sf::Color::Red);
    }
    
    void update() {
        sf::Vector2f tsize = shape.getSize();
        if (tsize.x > 0) {
            tsize.x -= 0.05;
            shape.setSize(sf::Vector2f(tsize.x, tsize.y));
        }

    }
    
    float hor_size() { return shape.getSize().x;};
    
};


int main(int, char const**) {
    std::srand(std::time(0));

    /* *****************************************
             Create the main window
     ***************************************** */

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Timber Clone");
     /* *****************************************
                 Load the background
     ***************************************** */

    sf::Texture background;
    if (!background.loadFromFile(resourcePath() + "graphics/background.png")) {
        return EXIT_FAILURE;
    }
    sf::Vector2u bimagesize = background.getSize();
    sf::Sprite backgroundSprite(background);
    backgroundSprite.setPosition(sf::Vector2f(0.f, 0.f));
    float xbackgroundscale = static_cast<float>(windowWidth)/bimagesize.x;
    float ybackgroundscale = static_cast<float>(windowHeight)/bimagesize.y;
    backgroundSprite.setScale(sf::Vector2f(xbackgroundscale, ybackgroundscale));
    
    /* *****************************************
                Create the Tree Sprite
     ***************************************** */

    sf::Texture tree;
    if(!tree.loadFromFile(resourcePath() + "graphics/tree.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite treeSprite(tree);
    treeSprite.setScale(sf::Vector2f(xbackgroundscale, ybackgroundscale));
    sf::Vector2u treesize = tree.getSize();
    treeSprite.setPosition(sf::Vector2f(windowWidth/2.f - (treesize.x*xbackgroundscale)/2.f, 0.f));
    
    /* *****************************************
                   Prepare the Bee
     ***************************************** */

    sf::Texture bee;
    if(!bee.loadFromFile(resourcePath() + "graphics/bee.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite beeSprite(bee);
    beeSprite.setScale(xbackgroundscale, ybackgroundscale);
    beeSprite.setPosition(450., 320.);
    // Set a speed for the bee to move
    float beeSpeed = 0.f;
    // Set a float to check is the bee is moving or not
    bool beeActive = false;
    
    /* *****************************************
         Prepare 3 Cloud sprites from 1 texture
     ***************************************** */
    sf::Texture cloud;
    if(!cloud.loadFromFile(resourcePath() + "graphics/cloud.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite cloudSprite1(cloud);
    sf::Sprite cloudSprite2(cloud);
    sf::Sprite cloudSprite3(cloud);
    cloudSprite1.setScale(xbackgroundscale, ybackgroundscale);
    cloudSprite2.setScale(xbackgroundscale, ybackgroundscale);
    cloudSprite3.setScale(xbackgroundscale, ybackgroundscale);
    cloudSprite1.setPosition(0.f, 0.f);
    cloudSprite2.setPosition(0.f, 75.f);
    cloudSprite3.setPosition(0.f, 150.f);
    // Are the clouds on screen
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    // Clouds' speed
    float cloud1Speed = 0.f;
    float cloud2Speed = 0.f;
    float cloud3Speed = 0.f;
    
    /* *****************************************
           Variables to control time (fps)
     ***************************************** */
    sf::Clock myclock;
    
    /* *****************************************
          Is the game running?
     ***************************************** */
    bool isgamePaused = true    ;

    /* *****************************************
     Set the score string
     ***************************************** */
    int score = 0;
    std::string scoreText = "Score = ";
    std::stringstream ss;
    ss << scoreText << score;

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "fonts/KOMIKAP_.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text(ss.str(), font, 50);
    text.setFillColor(sf::Color::White);
    
    /* *****************************************
     Set the time is up string
     ***************************************** */
    std::string message = "Time is Up!";
    
    // Create a graphical text to display
    sf::Text timeText(message, font, 50);
    sf::FloatRect textRect = timeText.getLocalBounds();
    timeText.setOrigin(textRect.left + textRect.width/2.f, textRect.top + textRect.height/2.f);
    timeText.setPosition(windowWidth/2.f, windowHeight/2.f);
    timeText.setFillColor(sf::Color::Red);
    
    
    /* *****************************************
     Set the rectangle timer
     ***************************************** */
    
    timerBar timer(timerWidth, timerHeight);
    
    /* *****************************************
     Set the branches
     ***************************************** */
    
    branches treeBranches(xbackgroundscale, ybackgroundscale);
    
    // Load a music to play
//    sf::Music music;
//    if (!music.openFromFile(resourcePath() + "nice_music.ogg")) {
//        return EXIT_FAILURE;
//    }

    // Play the music
//    music.play();

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                isgamePaused = !isgamePaused;
            }
            
        }
        //restart resets the counter and returns the
        //ammount of time that has elapsed
        sf::Time dt = myclock.restart();
        
        // Clear screen
        window.clear();
        
        if (!isgamePaused) {
            /* *****************************************
             Code to control the fps with sf::Clock
             ***************************************** */
            timer.update();

 
            
            // Setup the Bee
            if (!beeActive) {
                // How fast is the bee
                std::srand((int)std::time(0)*19);
                beeSpeed = (std::rand() % 100) + 100;
                // How high is the bee
                std::srand((int)std::time(0)*4);
                float beeHeight = (std::rand() % 300) + 300;
                beeSprite.setPosition(900, beeHeight);
                beeActive = true;
            } else {
                beeSprite.setPosition(beeSprite.getPosition().x - beeSpeed*dt.asSeconds(), beeSprite.getPosition().y);
                if (beeSprite.getPosition().x < -100) {
                    beeActive = false;
                }
            }
            
            // Setup Cloud 1
            if (!cloud1Active) {
                // How fast is the bee
                std::srand((int)std::time(0)*10);
                cloud1Speed = (std::rand() % 100) + 100;
                // How high is the bee
                std::srand((int)std::time(0)*10);
                float cloudHeight = (std::rand() % 50);
                cloudSprite1.setPosition(-100, cloudHeight);
                cloud1Active = true;
            } else {
                cloudSprite1.setPosition(cloudSprite1.getPosition().x + cloud1Speed*dt.asSeconds(), cloudSprite1.getPosition().y);
                if (cloudSprite1.getPosition().x > 900) {
                    cloud1Active = false;
                }
            }
            // Setup Cloud 2
            if (!cloud2Active) {
                // How fast is the bee
                std::srand((int)std::time(0)*9);
                cloud2Speed = (std::rand() % 100);
                // How high is the bee
                std::srand((int)std::time(0)*8);
                float cloudHeight = (std::rand() % 50)+50;
                cloudSprite2.setPosition(900, cloudHeight);
                cloud2Active = true;
            } else {
                cloudSprite2.setPosition(cloudSprite2.getPosition().x - cloud1Speed*dt.asSeconds(), cloudSprite2.getPosition().y);
                if (cloudSprite2.getPosition().x < -100) {
                    cloud2Active = false;
                }
            }
            // Setup Cloud 3
            if (!cloud3Active) {
                // How fast is the bee
                std::srand((int)std::time(0)*7);
                cloud3Speed = (std::rand() % 100) + 20;
                // How high is the bee
                std::srand((int)std::time(0)*13);
                float cloudHeight = (std::rand() % 50) + 100;
                cloudSprite3.setPosition(-100, cloudHeight);
                cloud3Active = true;
            } else {
                cloudSprite3.setPosition(cloudSprite3.getPosition().x + cloud3Speed*dt.asSeconds(), cloudSprite3.getPosition().y);
                if (cloudSprite3.getPosition().x > 900) {
                    cloud3Active = false;
                }
            }
        }
        
        
        // Draw the background
        window.draw(backgroundSprite);
        
        // Draw the clouds
        window.draw(cloudSprite1);
        window.draw(cloudSprite2);
        window.draw(cloudSprite3);
        // Draw the tree
        window.draw(treeSprite);
        
        
        //Draw the branches
        for (size_t i = 0; i < numberofBranches; ++i) {
            window.draw(treeBranches.branchSprite[i]);
            //            printf("%lu, %f\n",i, treeBranches.branchSprite[i].getPosition().y);
        }

        // Draw the bee
        window.draw(beeSprite);
        
        // Draw the string
        window.draw(text);
        
        // Setup the timer
        if (timer.hor_size() < 0.1) {
            isgamePaused = true;
            
            window.draw(timeText);
            
        }
        
        //Draw the timer
        window.draw(timer.shape);
        
        

        
        // Update the window
        window.display();

        
    }

    return EXIT_SUCCESS;
}
