#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define ENEMY 9

#pragma region 구조체
class ship
{
public:
	sf::RectangleShape shape;
	float speed = 0.1f;

	int HP = 5;
	int Maxhp = 5;

	sf::RectangleShape HPbar;
	sf::RectangleShape HPbarless;

	ship()
	{
		shape.setSize({ 20,20 });
		shape.setFillColor(sf::Color::Green);
		shape.setPosition({ 500,1100 });

		HPbarless.setSize(sf::Vector2f(200, 20));
		HPbarless.setFillColor(sf::Color::White);
		HPbarless.setPosition({20, 1100});

		HPbar.setSize(sf::Vector2f(200, 20));
		HPbar.setFillColor(sf::Color::Green);
		HPbar.setPosition({ 20, 1100 });
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

	void HPdraw(sf::RenderWindow &window)
	{
		float hpPercent = HP * 1.0f / Maxhp;
		HPbar.setSize(sf::Vector2f(200 * hpPercent, 20));

		window.draw(HPbarless);
		window.draw(HPbar);
	}

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

		//흘러간 시간을 반환하는 함수.

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

void removebullet(vector<bullet>& bullets)
{
	for (int i = 0; i < bullets.size();)
	{
		if (!bullets[i].active1())
		{
			bullets.erase(bullets.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

class Enemy
{
public:
	sf::RectangleShape enemy;
	float speed = 0.12f;


	vector<bullet> bullets;
	sf::Clock shoot;
	float delay = 1.0f;

	Enemy(float x, float y)
	{
		enemy.setSize({ 30,30 });
		enemy.setFillColor(sf::Color::Red);
		enemy.setPosition({ x,y });
	}

	void move()
	{
		enemy.move({ 0,speed });
		float random = rand() % 970;
		float stop1 = 450;

		//if (enemy.getPosition().y < stop1)
		//{
		//	enemy.move({0, speed });
		//}
		//else
		//{
		//	enemy.setPosition({ enemy.getPosition().x, stop1 });
		//}

		if (enemy.getPosition().y > 1200)
		{
			enemy.setPosition({ random, -30 });
		}

	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(enemy);
	}

	void hit()
	{

	}


};

sf::Clock bullet::clock;
float bullet::lastTime = 0.0f;

#pragma endregion

int main()
{
	sf::RenderWindow window(sf::VideoMode({1000,1200}), "SFML Window");
	srand(time(NULL));


		ship space;
		vector<bullet> bullets;
		vector<Enemy> enemy;

		for (int i = 0; i < ENEMY;i++)
		{
			float rx = rand() & 970;
			Enemy e(rx, -i * 150);
			enemy.push_back(e);
		}

		while (window.isOpen())
		{
			while (const std::optional event = window.pollEvent())
			{
				//std::optional : 이벤트가 존재할 경우에만 처리합니다.

				// is<sf::Event::Closed>() : 이벤트의 형태가 closed인지 확인합니다.
				if (event->is<sf::Event::Closed>())
				{

					window.close();
				}
				else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
				{
					//getIf<sf::Event::KeyPressed()> : 이벤트의 키보트 키가 입력되었는지 확인
					if (key->scancode == sf::Keyboard::Scancode::Escape)
					{
						window.close();
					}
				}
				// isOpen() : window가 열려 있는지 확인하는 함수
				//pollevent(): 이벤트 큐에서 이벤트를 하나씩 꺼내서 반환합니다.
				//	close() : window를 종료합니다.
			}





			space.Move();
			window.clear(sf::Color::Black);
#pragma region 적 및 총알
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && bullet::Shoot())
			{
				sf::Vector2f pos = space.shape.getPosition();
				bullet b(pos.x + 9, pos.y);
				bullets.push_back(b);
			}

			for (int i = 0; i < bullets.size(); ++i)
			{
				bullets[i].moving();
			}

			

			for (int i = 0; i < enemy.size(); i++)
			{
				enemy[i].move();
				enemy[i].draw(window);
			}

			for (int i = 0; i < bullets.size(); ++i)
			{
				bullets[i].draw(window);
			}




#pragma endregion
			window.draw(space.shape);
			space.HPdraw(window);
			removebullet(bullets);
			window.display();
		}





	return 0;
}


//colision detection

