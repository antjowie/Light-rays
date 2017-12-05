#include "Polygon.h"
#include "LightCircle.h"
#include "CollisionManager.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>

#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window;
	sf::Event event;
	CollisionManager collisionManager;

	sf::Texture temp;
	
	// For some reason quads doesn't support conclave shapes, quite a bummer, because I wanted to test it
	{
		// Init some simple shapes
		Polygon *shape1 = new Polygon(sf::PrimitiveType::Quads, temp);
		shape1->addVertex(sf::Vector2f(0, 0));
		shape1->addVertex(sf::Vector2f(200, -48));
		shape1->addVertex(sf::Vector2f(146, 115));
		shape1->addVertex(sf::Vector2f(70, 175));
		shape1->setPosition(120, 162);
		collisionManager.push(shape1);

		Polygon *shape2 = new Polygon(sf::PrimitiveType::Quads, temp);
		shape2->addVertex(sf::Vector2f(0, 0));
		shape2->addVertex(sf::Vector2f(185, -40));
		shape2->addVertex(sf::Vector2f(155, 255));
		shape2->addVertex(sf::Vector2f(10, 80));
		shape2->setPosition(520, 150);
		collisionManager.push(shape2);

		Polygon *shape3 = new Polygon(sf::PrimitiveType::Quads, temp);
		shape3->addVertex(sf::Vector2f(0, 0));
		shape3->addVertex(sf::Vector2f(160, 45));
		shape3->addVertex(sf::Vector2f(215, 210));
		shape3->addVertex(sf::Vector2f(-15, 170));
		shape3->setPosition(150,520);
		collisionManager.push(shape3);

		Polygon *shape4 = new Polygon(sf::PrimitiveType::Quads, temp);
		shape4->addVertex(sf::Vector2f(0, 0));
		shape4->addVertex(sf::Vector2f(100, 15));
		shape4->addVertex(sf::Vector2f(150, 80));
		shape4->addVertex(sf::Vector2f(15, 180));
		shape4->setPosition(490, 540);
		collisionManager.push(shape4);
	}

	LightCircle *mouse = new LightCircle(400,400);
	collisionManager.push(mouse);

	window.setFramerateLimit(60);
	window.create(sf::VideoMode(800, 800), "Light rays test", sf::Style::Close);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				break;
			
			case sf::Event::MouseMoved:
				mouse->setPosition(event.mouseMove.x, event.mouseMove.y);
				break;
			}
	
		window.clear(sf::Color(180,180,180));

		collisionManager.update(0.f);

		window.draw(collisionManager);

		window.display();
	}
	return 0;
}