#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;


class ship
{
public:
	sf::RectangleShape shape;

	float speed = 0.08f;

	ship()
	{
		shape.setSize({ 20,20 });
		shape.setFillColor(sf::Color::Red);
		shape.setPosition({ 500,1200 });
	};

	void Move()
	{

		sf::Vector2f movement(0, 0);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
			{
				movement.x -= speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
			{
				movement.x += speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
			{
				movement.y -= speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
			{
				movement.y += speed;
			}
	
		shape.move(movement);
	};


};

class bullet
{
public:
	sf::RectangleShape shape;
	float speeds = 0.25f;
	bool active = true;

	static sf::Clock clock;
	static float lastTime;
	static constexpr float Delay = 0.15f;

	bullet(float x, float y)
	{
		shape.setSize({ 2,2 });
		shape.setFillColor(sf::Color::Yellow);
		shape.setPosition({ x,y });
	};
	void moving()
	{
		shape.move({ 0,-speeds });
		if (shape.getPosition().y < 0)
		{
			active = false;
		}
	}
	bool active1() const
	{
		return active;
	}
	void draw(sf::RenderWindow& window)
	{
		if (active)
		{
			window.draw(shape);
		}
	}
	static bool Shoot() 
	{
		float now = clock.getElapsedTime().asSeconds();

		if (now - lastTime >= Delay)
		{
			lastTime = now;
			return true;
		}
		else
		{
			return false;
		}
	}

};

class Enemy
{
public:
	sf::RectangleShape enemy;


	Enemy()
	{
		
	}

};

sf::Clock bullet::clock;
float bullet::lastTime = 0.0f;

int main()
{
	sf::RenderWindow window(sf::VideoMode({1000,1400}), "SFML Window");
		
		ship space;
		vector<bullet> bullets;


		while (window.isOpen())
		{
			while (const std::optional event = window.pollEvent())
			{
				//std::optional : �̺�Ʈ�� ������ ��쿡�� ó���մϴ�.

				// is<sf::Event::Closed>() : �̺�Ʈ�� ���°� closed���� Ȯ���մϴ�.
				if (event->is<sf::Event::Closed>())
				{

					window.close();
				}
				else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
				{
					//getIf<sf::Event::KeyPressed()> : �̺�Ʈ�� Ű��Ʈ Ű�� �ԷµǾ����� Ȯ��
					if (key->scancode == sf::Keyboard::Scancode::Escape)
					{
						window.close();
					}
				}
				// isOpen() : window�� ���� �ִ��� Ȯ���ϴ� �Լ�
				//pollevent(): �̺�Ʈ ť���� �̺�Ʈ�� �ϳ��� ������ ��ȯ�մϴ�.
				//	close() : window�� �����մϴ�.
			}


			space.Move();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && bullet::Shoot())
			{
				sf::Vector2f pos = space.shape.getPosition();
				bullets.emplace_back(pos.x + 9, pos.y);
			}

			for (auto& b : bullets) b.moving();

			

			window.clear(sf::Color::Black);
			for (auto& b : bullets) b.draw(window);
			window.draw(space.shape);
			window.display();
		}


	return 0;
}