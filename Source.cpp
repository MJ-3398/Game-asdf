#include <iostream>
#include <SFML/Graphics.hpp>

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
	float speed = 0.1f;

	bullet()
	{
		shape.setSize({ 2,2 });
		shape.setFillColor(sf::Color::Yellow);
		shape.setPosition({ 500,1200 });
	};

};

#pragma region 라이브러리
// 정적 라이브러리(.lib)
// 프로그램 자체에 라이브러리 적용



// 동적 라이브러리(.dll)
// 외부 라이브러리 불러옴
// 크기가 커지지 않음
// 유연한 사용 가능
// 업데이트가 용이함
#pragma endregion

int main()
{
	sf::RenderWindow window(sf::VideoMode({1000,1400}), "SFML Window");
		
		ship space;

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
			}
			// isOpen() : window가 열려 있는지 확인하는 함수
			//pollevent(): 이벤트 큐에서 이벤트를 하나씩 꺼내서 반환합니다.
			//	close() : window를 종료합니다.


			space.Move();

			window.clear(sf::Color::Black);

			window.draw(space.shape);
			window.display();
		}
	return 0;
}