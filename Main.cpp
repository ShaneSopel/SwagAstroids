#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>


#include "Include/Animation.h"
#include "Include/Entity.h"
#include "Include/GameObject.h"
#include "Include/GameOver.h"
#include "Include/Level.h"
#include "Include/Menu.h"
#include "Include/SpaceConstants.h"
#include "Include/SpaceShip.h"
#include "Include/TextManager.h"

using namespace SpaceConstants;



int main()
{
    Menu menu(MAP_WIDTH1, MAP_HEIGHT1);
    Level level(MAP_WIDTH1, MAP_HEIGHT1);
    GameOver gover(MAP_WIDTH1, MAP_HEIGHT1);

    GameObject game;

    sf::Music music;    
    sf::SoundBuffer sblaser;
    sf::SoundBuffer sbExplosion;
    sf::Sound Lasersound;
    sf::Sound Explosionsound1; 

    TextManager oneuptext;
    TextManager HighScore;
    TextManager AsteroidsRemain;
    TextManager Lives;
    oneuptext.LoadFont();
    HighScore.LoadFont();
    Lives.LoadFont();
    AsteroidsRemain.LoadFont();

    sblaser.loadFromFile("/home/shanes/c++/SwagAsteroids/Resources/Sounds/Laser1.wav");
    sbExplosion.loadFromFile("/home/shanes/c++/SwagAsteroids/Resources/Sounds/Explosion-Hard.wav");
    Lasersound.setBuffer(sblaser);
    Explosionsound1.setBuffer(sbExplosion);

    music.openFromFile("/home/shanes/c++/SwagAsteroids/Resources/Sounds/SwagAsteroidsTheme.wav");
    music.setVolume(75);
    music.play();

    game.SetState(0);
    game.setAnimation();
    Animation sRock = game.getRock();
    Animation sSpaceShip = game.getSpaceShip();
    Animation sBullet = game.getBullet();
    Animation sExplosion = game.getExplosion();

    std::list<Entity *> entities;

    int val;
    val = 5;

    level.AsteroidHandler(sRock, entities);
    std::cout << entities.size();
    val = entities.size();

    std::cout << val << std::endl;

    SpaceShip *p = new SpaceShip();
    p->settings(sSpaceShip, 400, 400, 0, 20);
    entities.push_back(p);

    std::cout<< entities.size() << std::endl;

    sf::RenderWindow mainMenu(sf::VideoMode(MAP_WIDTH1, MAP_HEIGHT1), "SwagAsteroids");
    mainMenu.setFramerateLimit(60);

    while (mainMenu.isOpen())
    {

        sf::Event event;
        if (game.GetState() == 0)
        {
            menu.ProcessInput(mainMenu, event);
            menu.Update(mainMenu, game);
            menu.Draw(mainMenu);
        }

        if (game.GetState() == 1)
        {
            std::cout<< "made it here" <<std::endl;
            sf::RenderWindow play(sf::VideoMode(MAP_WIDTH1, MAP_HEIGHT1), "SwagAsteroids");
            sf::Event event1;
            while (play.isOpen())
            {
                level.ProcessInput(play, event1, Lasersound, entities, sBullet, p);
                level.Update(play, game, entities, oneuptext, HighScore, Lives, p, sSpaceShip, sExplosion, Explosionsound1, AsteroidsRemain);
                level.Draw(play,game, entities, oneuptext, HighScore, Lives, AsteroidsRemain, p);

                mainMenu.close();
            }

            if (game.GetState() == 2)
            {
                std::cout << "made it here";
                sf::RenderWindow GameOver(sf::VideoMode(MAP_WIDTH1, MAP_HEIGHT1), "SwagAsteroids");
                sf::Event over;

                while (GameOver.isOpen())
                {
                    gover.ProcessInput(GameOver, over);
                    gover.Update(GameOver, game);
                    gover.Draw(GameOver);
                }
            }
        }
    }
}