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

	int HP = 10;
	int Maxhp = 10;

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
		shape.move({ 0, -speeds });
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
		if (active == true)
		{
			window.draw(shape);
		}
	}
	static bool Shoot()
	{
		float now = clock.getElapsedTime().asSeconds();

		//getElapsedTime = 흘러간 시간을 반환하는 함수.

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

		if (enemy.getPosition().y > 1200)
		{
			enemy.setPosition({ random, -30 });
		}

	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(enemy);
	}



};

bool collision(sf::RectangleShape &hit, sf::RectangleShape &fit)
{
	sf::Vector2f hitP = hit.getPosition();
	sf::Vector2f fitP = fit.getPosition();

	sf::Vector2f hitS = hit.getSize();
	sf::Vector2f fitS = fit.getSize();

	sf::Vector2f centerH = { hitP.x + hitS.x / 2, hitP.y + hitS.y / 2 };
	sf::Vector2f centerF = { fitP.x + fitS.x / 2, fitP.y + fitS.y / 2 };

	float radiusH = std::min(hitS.x, hitS.y) / 2;
	float radiusF = std::min(fitS.x, fitS.y) / 2;

	float dx = centerH.x - centerF.x;
	float dy = centerH.y - centerF.y;
	float distance = sqrt(dx * dx + dy * dy);

	return distance <= (radiusH + radiusF);


}

sf::Clock bullet::clock;
float bullet::lastTime = 0.0f;

class Boss
{
public:
	sf::RectangleShape shape;
	sf::RectangleShape Larm;
	sf::RectangleShape Rarm;
	sf::RectangleShape LHand;
	sf::RectangleShape RHand;
	sf::RectangleShape body;
	sf::RectangleShape gun;
	sf::CircleShape a1;
	sf::CircleShape a2;
	sf::CircleShape a3;

	int HP = 120;
	int Maxhp = 120;

	sf::RectangleShape HPbar;
	sf::RectangleShape HPbarless;

	sf::Clock randmove;

	float speedX = 0.5f;
	float speedY = 0.3f;
	int dirX = 1;
	int dirY = 1;
	float spawnY;



	Boss()
	{
#pragma region bossshape
		shape.setSize({ 300,200 });
		shape.setFillColor(sf::Color::Red);
		shape.setPosition({ 350,100 });
		spawnY = shape.getPosition().y;

		Larm.setSize({ 80,40 });
		Larm.setFillColor(sf::Color::Red);
		Larm.setPosition({ 270,170 });
		Rarm.setSize({ 80,40 });
		Rarm.setFillColor(sf::Color::Red);
		Rarm.setPosition({ 650,170 });

		LHand.setSize({ 100,200 });
		LHand.setFillColor(sf::Color::Red);
		LHand.setPosition({ 170,140 });
		RHand.setSize({ 100,200 });
		RHand.setFillColor(sf::Color::Red);
		RHand.setPosition({ 730,140 });

		body.setSize({ 120,230 });
		body.setFillColor(sf::Color::Red);
		body.setPosition({ 440,300 });

		a1.setRadius({80});
		a1.setOrigin({ 80, 80 });
		a1.setFillColor(sf::Color::Blue);
		a1.setPosition({ 500,210 });
		a1.setOutlineColor(sf::Color::Black);
		a1.setOutlineThickness({ 10 });


		a2.setRadius({ 30 });
		a2.setOrigin({ 30, 30 });
		a2.setFillColor(sf::Color::Blue);
		a2.setPosition({ 220,210 });
		a2.setOutlineThickness({ 7 });
		a2.setOutlineColor(sf::Color::Black);

		a3.setRadius({ 30 });
		a3.setOrigin({ 30, 30 });
		a3.setFillColor(sf::Color::Blue);
		a3.setPosition({ 780,210 });
		a3.setOutlineThickness({ 7 });
		a3.setOutlineColor(sf::Color::Black);
#pragma endregion

		HPbarless.setSize(sf::Vector2f(200, 20));
		HPbarless.setFillColor(sf::Color::White);
		HPbarless.setPosition({ 50, 10 });

		HPbar.setSize(sf::Vector2f(200, 20));
		HPbar.setFillColor(sf::Color::Blue);
		HPbar.setPosition({ 50, 10 });
	}

	void MovingDirection()
	{
		if (randmove.getElapsedTime().asSeconds() >= 1.0f)
		{
			dirX = (rand() % 2 == 0) ? 1 : -1;
			dirY = (rand() % 2 == 0) ? 1 : -1;
			speedX = 0.1f + float(rand() % 20)/100;
			speedY = 0.1f + float(rand() % 20)/100;
			randmove.restart();
		}
	}

	void move()
	{
		MovingDirection();

		sf::Vector2f b1 = shape.getPosition();

		b1.x += speedX * dirX;
		if (b1.x <= 0) 
		{ 
			b1.x = 0; dirX = 1; 
		}
		if (b1.x + shape.getSize().x >= 1000) 
		{
			b1.x = 1000 - shape.getSize().x; dirX = -1; 
		}

		b1.y += speedY * dirY;
		if (b1.y <= spawnY - 200)
		{
			b1.y = spawnY - 200; dirY = 1;
		}
		if (b1.y >= spawnY + 200) 
		{ 
			b1.y = spawnY + 200; dirY = -1; 
		}

		shape.setPosition(b1);

		Larm.setPosition(sf::Vector2f(b1.x - 80, b1.y + 70));
		Rarm.setPosition(sf::Vector2f(b1.x + 300, b1.y + 70));
		LHand.setPosition(sf::Vector2f(b1.x - 180, b1.y + 40));
		RHand.setPosition(sf::Vector2f(b1.x + 380, b1.y + 40));
		body.setPosition(sf::Vector2f(b1.x + 90, b1.y + 200));

		a1.setPosition(sf::Vector2f(b1.x + 150, b1.y + 110));
		a2.setPosition(sf::Vector2f(b1.x - 130, b1.y + 110));
		a3.setPosition(sf::Vector2f(b1.x + 430, b1.y + 110));

	}

	void HPdraw(sf::RenderWindow& window)
	{
		float hpPercent = HP * 1.0f / Maxhp;

		float MaxHP1 = Maxhp * 1.0f / Maxhp;

		HPbar.setSize(sf::Vector2f(900 * hpPercent, 20));
		HPbarless.setSize(sf::Vector2f(900 * MaxHP1, 20));
		window.draw(HPbarless);
		window.draw(HPbar);
	}
};

bool bosscollision(sf::RectangleShape& hit, sf::CircleShape& fit)
{
	sf::Vector2f hitP = hit.getPosition();
	sf::Vector2f fitP = fit.getPosition();

	sf::Vector2f hitS = hit.getSize();
	float fitS = fit.getRadius();

	sf::Vector2f centerH = { hitP.x + hitS.x / 2, hitP.y + hitS.y / 2 };


	float radiusH = std::min(hitS.x, hitS.y) / 2;
	

	float dx = centerH.x - fitP.x;
	float dy = centerH.y - fitP.y;
	float distance = sqrt(dx * dx + dy * dy);

	return distance <= (radiusH + fitS);


}


#pragma endregion

int main()
{
	sf::RenderWindow window(sf::VideoMode({1000,1200}), "SFML Window");
	srand(time(NULL));


		ship space;
		vector<bullet> bullets;
		vector<Enemy> enemy;
		Boss Boss;

		int a1 = 0; // 체력 회복용 변수

		sf::Clock Bossspawn;
		bool spawn = false;

		for (int i = 0; i < ENEMY;i++)
		{
			float rx = rand() % 970;
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


			float spawnboss = Bossspawn.getElapsedTime().asSeconds();
			if (spawnboss >= 1.0f)
			{
				spawn = true;
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

			if (spawn == false)
			{
				for (int i = 0; i < enemy.size(); i++)
				{
					enemy[i].move();
					enemy[i].draw(window);
					if (collision(space.shape, enemy[i].enemy))
					{
						space.HP -= 1;
						float rx = rand() % 970;
						enemy[i].enemy.setPosition({ rx, -30 });
					}
				}
			}

			for (int i = 0; i < bullets.size(); ++i)
			{
				bullets[i].draw(window);

				if (spawn == false)
				{
					for (int j = 0; j < enemy.size(); ++j)
					{

						if (collision(bullets[i].shape, enemy[j].enemy))
						{
							bullets[i].active = false;
							a1++;
							float rx = rand() % 970;
							enemy[j].enemy.setPosition({ rx, -30 });
							if (a1 % 10 == 0 && space.HP < 5)
							{
								space.HP++;

							}
						}
					}
				}
				if (spawn == true && bosscollision(bullets[i].shape, Boss.a1))
				{
					bullets[i].active = false;
					Boss.HP -= 1;
				}
				if (spawn == true && bosscollision(bullets[i].shape, Boss.a2))
				{
					bullets[i].active = false;
					Boss.HP -= 1;
				}
				if (spawn == true && bosscollision(bullets[i].shape, Boss.a3))
				{
					bullets[i].active = false;
					Boss.HP -= 1;
				}
			}

#pragma endregion
			window.draw(space.shape);
			if (spawn == true)
			{
				Boss.move();
				window.draw(Boss.shape);
				window.draw(Boss.Larm);
				window.draw(Boss.Rarm);
				window.draw(Boss.RHand);
				window.draw(Boss.LHand);
				window.draw(Boss.body);
				window.draw(Boss.a1);
				window.draw(Boss.a2);
				window.draw(Boss.a3);
				Boss.HPdraw(window);
			}

			space.HPdraw(window);
			removebullet(bullets);
			window.display();
		}


	return 0;
}
