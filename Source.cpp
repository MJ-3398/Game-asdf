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

#pragma region ���̺귯��
// ���� ���̺귯��(.lib)
// ���α׷� ��ü�� ���̺귯�� ����



// ���� ���̺귯��(.dll)
// �ܺ� ���̺귯�� �ҷ���
// ũ�Ⱑ Ŀ���� ����
// ������ ��� ����
// ������Ʈ�� ������
#pragma endregion

int main()
{
	sf::RenderWindow window(sf::VideoMode({1000,1400}), "SFML Window");
		
		ship space;

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
			}
			// isOpen() : window�� ���� �ִ��� Ȯ���ϴ� �Լ�
			//pollevent(): �̺�Ʈ ť���� �̺�Ʈ�� �ϳ��� ������ ��ȯ�մϴ�.
			//	close() : window�� �����մϴ�.


			space.Move();

			window.clear(sf::Color::Black);

			window.draw(space.shape);
			window.display();
		}
	return 0;
}