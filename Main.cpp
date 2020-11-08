#include <sfml\Graphics.hpp>
#include <sfml\Audio.hpp>
#include <iostream>
#include <sstream> //for score


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Window title");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	//varaible that keeps the game loop running
	bool play = true;

	//font
	sf::Font font;
	if (font.loadFromFile("Data/arial.ttf") == false)
	{
		return 1;
	}
	sf::Text score;
	score.setFont(font);
	score.setCharacterSize(30);
	score.setFillColor(sf::Color(123, 145, 178));
	score.setPosition(350, 10);
	score.setString("0 : 0");

	//images
	sf::Texture texturePad;
	sf::Texture textureBall;
	sf::Texture textureBG;
	if (texturePad.loadFromFile("Data/pad.png") == false)
	{
		return -1;
	}
	if (textureBall.loadFromFile("Data/ball.png") == false)
	{
		return -1;
	}
	if (textureBG.loadFromFile("Data/background.png") == false)
	{
		return -1;
	}

	//sounds
	sf::SoundBuffer bufferHit;
	if (bufferHit.loadFromFile("Data/hit.wav") == false)
	{
		return -1;
	}
	sf::Sound hit;
	hit.setBuffer(bufferHit);
	hit.setVolume(25);

	//states
	bool upKey = false;
	bool downKey = false;

	//variables
	int yVelocityPad1 = 0;
	int xVelocityBall = -4;
	int yVelocityBall = -4;
	int yVelocityPad2 = 0;
	int pad1Score = 0;
	int pad2Score = 0;

	//shapes
	sf::RectangleShape background;
	background.setSize(sf::Vector2f(800, 600));
	background.setPosition(0, 0); //to make sure
	background.setTexture(&textureBG);
	sf::RectangleShape pad1;
	pad1.setSize(sf::Vector2f(50, 100));
	pad1.setPosition(50, 200);
	pad1.setTexture(&texturePad);
	sf::RectangleShape pad2;
	pad2.setSize(sf::Vector2f(50, 100));
	pad2.setPosition(700, 200);
	pad2.setTexture(&texturePad);
	sf::RectangleShape ball; //bc collison
	ball.setSize(sf::Vector2f(50, 50));
	ball.setPosition(400, 200);
	ball.setTexture(&textureBall);

	//main game loop
	while (play == true)
	{
		sf::Event event;
		//events
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) //if the event was closing the window, close the window
			{
				play = false;
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
			{
				upKey = true;
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
			{
				downKey = true;
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up)
			{
				upKey = false;
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down)
			{
				downKey = false;
			}
		}

		////logic
		//pad1
		if (upKey)
		{
			yVelocityPad1 = -5;
		}
		if (downKey)
		{
			yVelocityPad1 = 5;
		}
		if ((upKey && downKey) || (!upKey && !downKey))
		{
			yVelocityPad1 = 0;
		}

		pad1.move(0, yVelocityPad1);
		//out of bounds check
		if (pad1.getPosition().y < 0)
		{
			pad1.setPosition(50, 0);
		}
		if (pad1.getPosition().y > 500)
		{
			pad1.move(0, -yVelocityPad1);
		}

		//pad2, ai logic
		if (ball.getPosition().y < pad2.getPosition().y)
		{
			yVelocityPad2 = -3;
		}
		if (ball.getPosition().y > pad2.getPosition().y)
		{
			yVelocityPad2 = 3;
		}
		pad2.move(0, yVelocityPad2);
		/* //out of bounds check, not needed bc ball basically controls movement
		if (pad2.getPosition().y < 0)
		{
			pad2.setPosition(50, 0);
		}
		if (pad2.getPosition().y > 500)
		{
			pad2.move(0, -yVelocityPad2);
		}*/

		//ball
		ball.move(xVelocityBall, yVelocityBall);
		//out of bounds check
		if (ball.getPosition().y < 0)
		{
			yVelocityBall = -yVelocityBall;
		}
		if (ball.getPosition().y > 550)
		{
			yVelocityBall = -yVelocityBall;
		}
		if (ball.getPosition().x < -50)
		{
			++pad2Score;
			ball.setPosition(300, 200);
		}
		if (ball.getPosition().x > 800)
		{
			++pad1Score;
			ball.setPosition(300, 200);
		}
		//collision with pad1
		if (ball.getGlobalBounds().intersects(pad1.getGlobalBounds()))
		{
			xVelocityBall = -xVelocityBall;
			hit.play();
		}
		//collision with pad2
		if (ball.getGlobalBounds().intersects(pad2.getGlobalBounds()))
		{
			xVelocityBall = -xVelocityBall;
			hit.play();
		}

		//rendering
		window.clear();
		window.draw(background);
		window.draw(pad1);
		window.draw(pad2);
		window.draw(ball);
		//score setting
		std::stringstream text;
		if (pad1Score == 5 && pad2Score < 5)
		{
			text << "WIN!";
			play = false;
		}
		else if (pad2Score == 5 && pad1Score < 5)
		{
			text << "GAME OVER!";
			score.setPosition(300, 10);
			play = false;
		}
		else
		{
			text << pad1Score << " : " << pad2Score;
		}
		score.setString(text.str());
		//score drawing
		window.draw(score);
		window.display();
	}
	return 0;
}