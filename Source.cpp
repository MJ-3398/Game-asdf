#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define ENEMY 40
#define BossHP 80
#pragma region 구조체
class ship
{
public:
	sf::RectangleShape shape;
	float speed = 0.14f;

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
		HPbar.setSize({ 200 * hpPercent, 20 });
		window.draw(HPbarless);
		window.draw(HPbar);
	}

};

class bullet
{
public:
	sf::RectangleShape shape;
	float speeds;
	bool active = true;
	int direction;

	static sf::Clock clock;
	static float lastTime;
	static constexpr float Delay = 0.16f;

	bullet(float x, float y, int d = -1, float s = 0.15f)
	{
		shape.setSize({ 8,8 });
		shape.setFillColor(sf::Color::Yellow);
		shape.setPosition({ x,y });
		direction = d;
		speeds = s;
	};

	void moving()
	{
		shape.move({ 0, speeds*direction });
		if (shape.getPosition().y < 0 || shape.getPosition().y > 1200)
		{
			active = false;
		}
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

void removebullet(vector<bullet>& bullets, float a = 1200)
{
	for (int i = bullets.size() - 1; i >= 0; i--)
	{
		sf::Vector2f coord = bullets[i].shape.getPosition();

		if (!bullets[i].active || coord.y < -10 || coord.y > a + 10)
		{
			bullets.erase(bullets.begin() + i);
		}
	}
}

class Enemy
{
public:
	sf::RectangleShape enemy;

	sf::Clock randmove;
	float speedX = 0.03f;
	float speedY = 0.05f;
	int dirX = 1;

	Enemy(float x, float y)
	{
		enemy.setSize({ 30,30 });
		enemy.setFillColor(sf::Color::Red);
		enemy.setPosition({ x,y });
	}

	void move()
	{
		enemy.move({ speedX * dirX ,speedY });

		if (randmove.getElapsedTime().asSeconds() >= 1.0f)
		{
			dirX = (rand() % 2 == 0) ? 1 : -1;
			speedX = 0.05f + float(rand() % 10) / 100;
			randmove.restart();
		}

		if (enemy.getPosition().y > 1200)
		{
			float random = rand() % 970;
			enemy.setPosition({ random, -30 });
		}

		if (enemy.getPosition().x <= 0)
		{
			enemy.setPosition({ 0, enemy.getPosition().y });
			dirX = 1;
		}
		if (enemy.getPosition().x + enemy.getSize().x >= 1000)
		{
			enemy.setPosition({ 1000 - enemy.getSize().x, enemy.getPosition().y });
			dirX = -1;
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
#pragma region 보스 형태
	sf::RectangleShape shape;
	sf::RectangleShape Larm;
	sf::RectangleShape Rarm;
	sf::RectangleShape LHand;
	sf::RectangleShape RHand;
	sf::RectangleShape body;
	sf::CircleShape a1;
	sf::CircleShape a2;
	sf::CircleShape a3;

	sf::RectangleShape gun1;
	sf::RectangleShape gun2;
	sf::RectangleShape gun3;
	sf::RectangleShape gun4;
	sf::RectangleShape gun5;

	int HP = BossHP;
	int Maxhp = BossHP;

	sf::RectangleShape HPbar;
	sf::RectangleShape HPbarless;
#pragma endregion

	sf::Clock randmove;

	
	float speedX = 0.4f;
	float speedY = 1.2f;
	int dirX = 1;
	int dirY = 1;
	float spawnY;

	vector<bullet> bullets;
	sf::Clock shoot;
	float rs = rand() % 970;
	float delay = 0.2f;

	sf::Vector2f Size = { 20,80 };
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

		gun1.setSize(Size);
		gun1.setFillColor(sf::Color::White);
		gun2.setSize(Size);
		gun2.setFillColor(sf::Color::White);
		gun3.setSize(Size);
		gun3.setFillColor(sf::Color::White);
		gun4.setSize(Size);
		gun4.setFillColor(sf::Color::White);
		gun5.setSize({60, 120});
		gun5.setFillColor(sf::Color::White);
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
			speedX = 0.1f + float(rand() % 5)/1000;
			speedY = 0.1f + float(rand() % 5)/1000;
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
		if (b1.y <= spawnY - 100)
		{
			b1.y = spawnY - 100; dirY = 1;
		}
		if (b1.y >= spawnY + 100) 
		{ 
			b1.y = spawnY + 100; dirY = -1; 
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

		gun1.setPosition(sf::Vector2f(b1.x - 170, b1.y + 240));
		gun2.setPosition(sf::Vector2f(b1.x - 110, b1.y + 240));
		gun3.setPosition(sf::Vector2f(b1.x + 390, b1.y + 240));
		gun4.setPosition(sf::Vector2f(b1.x + 450, b1.y + 240));
		gun5.setPosition(sf::Vector2f(b1.x + 120, b1.y + 430));
	}

	void HPdraw(sf::RenderWindow& window)
	{
		float hpPercent = HP * 1.0f / Maxhp;

		HPbar.setSize(sf::Vector2f(900 * hpPercent, 20));
		HPbarless.setSize({ 900, 20 });
		window.draw(HPbarless);
		window.draw(HPbar);
	}

	void Reset()
	{
		HP = Maxhp;
		spawnY = 100;
		shape.setPosition({ 350, spawnY });
		Larm.setPosition({ 270,170 });
		Rarm.setPosition({ 650,170 });
		LHand.setPosition({ 170,140 });
		RHand.setPosition({ 730,140 });
		body.setPosition({ 440,300 });
		a1.setPosition({ 500,210 });
		a2.setPosition({ 220,210 });
		a3.setPosition({ 780,210 });
	}

	void shooting()
	{
		if (shoot.getElapsedTime().asSeconds() >= delay)
		{
			bullets.emplace_back(gun1.getPosition().x + gun1.getSize().x / 2, gun1.getPosition().y + gun1.getSize().y, 1, 0.03f);
			bullets.emplace_back(gun2.getPosition().x + gun2.getSize().x / 2, gun2.getPosition().y + gun2.getSize().y, 1, 0.05f);
			bullets.emplace_back(gun3.getPosition().x + gun3.getSize().x / 2, gun3.getPosition().y + gun3.getSize().y, 1, 0.045f);
			bullets.emplace_back(gun4.getPosition().x + gun4.getSize().x / 2, gun4.getPosition().y + gun4.getSize().y, 1, 0.04f);
			bullets.emplace_back(gun5.getPosition().x + gun5.getSize().x / 2, gun5.getPosition().y + gun5.getSize().y, 1, 0.035f);
			shoot.restart();
		}

	}

	void MoveBullets()
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i].moving();
		}
		for (int i = bullets.size() - 1; i >= 0; i--)
		{
			if (!bullets[i].active)
			{
				bullets.erase(bullets.begin() + i);
			}
		}
	}
	void DrawBullets(sf::RenderWindow& window)
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i].draw(window);
		}
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

void Explain(sf::RenderWindow& window, sf::Font& font)
{
	sf::Text controls(font, sf::String("Game Clear\n Press 'R' to Restart Game"), 48);
	controls.setFont(font);
	controls.setCharacterSize(20);
	controls.setFillColor(sf::Color::White);
	controls.setPosition({ 550, 1050 });

	controls.setString("If Player kills 10 enemies, Heal 1 time.\n If Player damages 20 times to boss, Heal 1 time.\n Arrow Key : Move\nSpace : Shoot");

	window.draw(controls);
}
int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1000,1200 }), "SFML Window");
	srand(time(NULL));

	ship space;
	vector<bullet> bullets;
	vector<Enemy> enemy;
	Boss Boss;

	for (int i = 0; i < ENEMY;i++)
	{
		float rx = rand() % 970;
		Enemy e(rx, -i * 150);
		enemy.push_back(e);
	}

	int a1 = 0; // 체력 회복용 변수들
	int a2 = 0;

#pragma region 텍스트 관련
	sf::Clock Bossspawn;
	bool spawn = false;
	float bosscount = 50.0f;

	sf::Font font;
	if (!font.openFromFile("arial.ttf"))
	{
		return -1;
	}
	sf::Text text(font, sf::String("0"), 24);
	text.setFillColor(sf::Color::White);
	text.setPosition({ 50, 1050 });

	sf::Text clear(font, sf::String("Game Clear\n Press 'R' to Restart Game"), 48);
	clear.setFillColor(sf::Color::White);
	clear.setPosition({ 250, 600 });

	sf::Text gameOver(font, sf::String("Game Over\n Press 'R' to Restart Game"), 48);
	gameOver.setFillColor(sf::Color::Red);
	gameOver.setPosition({ 250, 600 });

	bool bossalive = true;
	bool bossdie = false;
	bool gameover = false;
#pragma endregion

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

#pragma region 보스몹 생성/ 게임 리셋

		//게임 클리어(보스 제거 시)
		if (spawn == true && Boss.HP <= 0)
		{
			bossalive = false;
			bossdie = true;
		}
		//게임 패배(체력 끝)
		if (space.HP <= 0)
		{
			gameover = true;
		}

		if (((bossdie == true) || (gameover == true)) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::R))
		{
			Boss.Reset();
			bossalive = true;
			bossdie = false;
			spawn = false;
			gameover = false;
			Bossspawn.restart();
			space.HP = space.Maxhp;
			bullets.clear();
			enemy.clear();

			for (int i = 0; i < ENEMY; i++)
			{
				float rx = rand() % 970;
				Enemy e(rx, -i * 150);
				enemy.push_back(e);
			}
			a1 = 0;
			a2 = 0;
		}

		float spawnboss = Bossspawn.getElapsedTime().asSeconds();
		float c1 = bosscount - spawnboss;
		if (c1 <= 1.0f)
		{
			c1 = 0.0f;
		}
		if (spawnboss >= 50.0f)
		{
			spawn = true;
		}
		if (c1 >= 0.0f)
		{
			text.setString(sf::String(std::to_string(c1)));
		}

#pragma endregion
		window.clear(sf::Color::Black);

		if (gameover == false)
		{
			space.Move();
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
			removebullet(bullets);

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
			Boss.MoveBullets();



			//적 충돌 및 회복
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
							if (a1 % 10 == 0 && space.HP < 10)
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
					a2++;
					if (a2 % 20 == 0 && space.HP < 10)
					{
						space.HP++;

					}
				}
				if (spawn == true && bosscollision(bullets[i].shape, Boss.a2))
				{
					bullets[i].active = false;
					Boss.HP -= 1;
					a2++;
					if (a2 % 20 == 0 && space.HP < 10)
					{
						space.HP++;

					}
				}
				if (spawn == true && bosscollision(bullets[i].shape, Boss.a3))
				{
					bullets[i].active = false;
					Boss.HP -= 1;
					a2++;
					if (a2 % 20 == 0 && space.HP < 10)
					{
						space.HP++;

					}
				}
			}

			//보스 충돌
			for (int i = 0; i < Boss.bullets.size(); i++)
			{
				if (collision(Boss.bullets[i].shape, space.shape))
				{
					Boss.bullets[i].active = false;
					space.HP--;
				}
			}

#pragma endregion
		}
		window.draw(space.shape);
		if ((spawn == true && bossalive == true) && gameover == false)
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
			window.draw(Boss.gun1);
			window.draw(Boss.gun2);
			window.draw(Boss.gun3);
			window.draw(Boss.gun4);
			window.draw(Boss.gun5);
			Boss.HPdraw(window);
			Boss.shooting();
			Boss.MoveBullets();
			Boss.DrawBullets(window);
		}

		space.HPdraw(window);
		window.draw(text);
		if (bossalive == false)
		{
			window.draw(clear);
		}

		if (gameover == true)
		{
			window.draw(gameOver);
		}

		Explain(window, font);
		window.display();
	}
	return 0;
}
