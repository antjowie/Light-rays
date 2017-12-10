#include "LightCircle.h"

#include <SFML\Graphics\Texture.hpp>

#include <SFML\Graphics\RenderTarget.hpp>

#include <iostream>

static sf::Texture tex;

void LightCircle::rotateVector(sf::Vector2f & vector, const float degrees) const
{
	float radians{ degrees / 180.f * 3.14159265359f };

	const float x{ vector.x * std::cos(radians) - vector.y * std::sin(radians) };
	const float y{ vector.x * std::sin(radians) + vector.y * std::cos(radians) };

	vector.x = x;
	vector.y = y;
}

void LightCircle::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform = getTransform();
	//states.texture = m_texture;
	target.draw(m_vertices, states);
	target.draw(m_circle);

	for (size_t i{ 0 }; i < m_vertices.getVertexCount(); i++)
	{
		sf::CircleShape temp(10);
		temp.setOrigin(10, 10);
		temp.setPosition(localToWorld(m_vertices[i].position));
		target.draw(temp);
	}

	return;
}

LightCircle::LightCircle(const float radius, const int rays):
	Object(sf::PrimitiveType::TriangleFan,tex,1), // Should happen in an init phase
	m_circle(0,rays)
{
	m_circle.setFillColor(sf::Color(80, 80, 80, 100));
	m_circle.setOutlineThickness(1);
	m_circle.setOutlineColor(sf::Color::Red);
	
	setRadius(radius);
	tex.loadFromFile("");
}

void LightCircle::hit()
{
}

void LightCircle::update(const float elapsedTime, const CollisionManager & collisionManager)
{
	/*
	m_vertices.clear();
	sf::Vector2f mouse{ getPosition() };
	sf::Vector2f origin{ getOrigin() };
	sf::Vector2f offset{ sf::Vector2f(600, 400)};
	setPosition(getOrigin());

	sf::Vector2f mag1{ mouse - origin};
	sf::Vector2f mag2{ 0,0};

	m_vertices.append(sf::Vertex(origin, sf::Color::Red));
	m_vertices.append(sf::Vertex(offset, sf::Color::Red));
	m_vertices.append(sf::Vertex(origin, sf::Color::Red));
	m_vertices.append(sf::Vertex(mouse,sf::Color::Red));
	
	double radians{ std::atan2(mag1.x, mag1.y) };

	std::cout << radians<< '\n';
	*/
	
	m_vertices.clear();
	m_circle.setPosition(getPosition());
	const Object *previousObject{ nullptr };
	Collided collided;

	m_vertices.append(sf::Vertex(getOrigin(), sf::Color::Red));

	for (size_t i{ 0 }; i <= m_circle.getPointCount(); i++)
	{
		sf::Vector2f ray(m_circle.getPoint(i % m_circle.getPointCount()));
		collided = collisionManager.getCollision(getOrigin(), ray, this);
		if (collided.object != nullptr && collided.object != previousObject)
		{
			previousObject = collided.object;
			std::list<sf::Vertex> vertices;
			
			// Load all vertices with corrected magnitude
			for (const auto &vertex : collided.object->getVertices())
			{
				// This rotates each vector a bit so that we can get behind the block edges
				sf::Vertex temp(vertex);
				temp.color = sf::Color::Red;
				temp.position = worldToLocal(collided.object->localToWorld(temp.position));

				// Turn into a unit vector and give radius as length
				sf::Vector2f movement{ temp.position - getOrigin() };
				movement = movement / std::sqrtf(movement.x * movement.x + movement.y * movement.y) * m_circle.getRadius();

				rotateVector(movement, -0.1f);
				vertices.push_back(sf::Vertex(worldToLocal(movement * collisionManager.getCollision(getPosition(), localToWorld(getOrigin() + movement), this).percentage + getPosition()),sf::Color::Red));
				rotateVector(movement, 0.2f);
				vertices.push_back(sf::Vertex(worldToLocal(movement * collisionManager.getCollision(getPosition(), localToWorld(getOrigin() + movement), this).percentage + getPosition()), sf::Color::Red));
			}
			
			// Push all vertices is correct order for SFML to render
			while (!vertices.empty())
			{
				// Check wether or not we loop around the circle
				bool leftPos{ false };
				bool rightPos{ false };
				bool looped{ false };
				for (const auto &it : vertices)
				{
					const sf::Vector2f movement{ it.position - getOrigin() };
					if (movement.y < 0 && movement.x > 0)
						rightPos = true;

					else if (movement.y < 0 && movement.x < 0)
						leftPos = true;
				}
				
				if (leftPos && rightPos)
					looped = true;

				// Can never be smaller than -pi
				float largest = -4.f;
				auto selected{ vertices.begin() };
				for (auto iter{vertices.begin()}; iter != vertices.end(); iter++)
				{
					double radians{ std::atan2(iter->position.x - getOrigin().x, iter->position.y- getOrigin().y) };
				
					if (looped && radians > 0)
						radians -= 10.f;

					if (radians > largest)
					{
				 		largest = radians;
						selected = iter;
					}
				}
				m_vertices.append(*selected);
				vertices.erase(selected);
			}
		}
		//else
			//m_vertices.append(sf::Vertex(ray, sf::Color::Red));
	}
	//
	// TODO
	// Calculate order of vertices
	// Option 1: get the difference of degrees for all vectors
	//
}

void LightCircle::setRadius(const float radius)
{
	m_circle.setRadius(std::abs(radius));
	m_circle.setOrigin(m_circle.getRadius(), m_circle.getRadius());
	setOrigin(m_circle.getRadius(),m_circle.getRadius());
}