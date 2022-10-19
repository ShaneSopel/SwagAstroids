
#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>

#include "Include/Asteroids.h"
#include "Include/Animation.h"
#include "Include/Bullet.h"
#include "Include/Collision.h"
#include "Include/DrawMap.h"
#include "Include/Entity.h"
#include "GameObject.h"
#include "Include/Menu.h"
#include "Include/SpaceConstants.h"
#include "Include/SpaceShip.h"

using namespace SpaceConstants;

int main()
{
    unsigned char level = 0;

   	sf::RenderWindow app(sf::VideoMode(MAP_WIDTH1, MAP_HEIGHT1), "SwagAsteroids");
    app.setFramerateLimit(60);

    Menu menu(app.getSize().x, app.getSize().y);
    GameObject game;
    game.setAnimation();
    Animation sRock = game.getRock();
    Animation sSpaceShip = game.getSpaceShip();
    Animation sBullet = game.getBullet();
    Animation sExplosion= game.getExplosion();

    std::list<Entity*> entities;

    for(int i=0;i<15;i++)
    {
      Asteroids *a = new Asteroids();
      a->settings(sRock, rand()%MAP_WIDTH1, rand()%MAP_HEIGHT1, rand()%360, 25);
      entities.push_back(a);
    }

    SpaceShip *p = new SpaceShip();
    p->settings(sSpaceShip,200,200,0,20);
    entities.push_back(p);

    while (app.isOpen())
    {
        sf::Event event;

        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            app.close();

            if (event.key.code == sf::Keyboard::Space)
            {
                Bullet *b = new Bullet();
                b->settings(sBullet,p->x_coord,p->y_coord,p->angle,10);
                entities.push_back(b);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) p->angle+=3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  p->angle-=3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    p->thrust=true;
        else p->thrust=false;

        for(auto a:entities)
        {

            for(auto b:entities)
            {
            if (a->name=="asteroid" && b->name=="bullet")
            if ( isCollide(a,b) )
                {
                    a->life=false;
                    b->life=false;

                    Entity *e = new Entity();
                    e->settings(sExplosion,a->x_coord,a->y_coord,0,0);
                    e->name="explosion";
                    entities.push_back(e);


                    for(int i=0;i<2;i++)
                    {
                        if (a->R==15) continue;
                        Entity *e = new Asteroids();
                        e->settings(sRock,a->x_coord,a->y_coord,rand()%360,15);
                        entities.push_back(e);
                    }

                }

            }
        }

        if (p->thrust)  p->anim = sSpaceShip;
        else   p->anim = sSpaceShip;

        for(auto e:entities)
        if (e->name=="explosion")
         if (e->anim.isEnd()) e->life=0;

        if (rand()%150==0)
        {
            Asteroids *a = new Asteroids();
            a->settings(sRock, 0,rand()%MAP_HEIGHT1, rand()%360, 25);
            entities.push_back(a);
        }

        for(auto i=entities.begin();i!=entities.end();)
        {
            Entity *e = *i;
            e->update();
            e->anim.update();

            if (e->life==false) {i=entities.erase(i); delete e;}
            else i++;
        }

        app.clear();
        draw_map(app);
 
        for(auto i:entities) i->draw(app);
        app.display();

    }
    
}