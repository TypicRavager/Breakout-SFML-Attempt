#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#define Screen_Width 1280
#define Screen_Height 720

class Entities {
	//Purpose is to allow ball to interact with not only pad but bricks
private:
	
public:
	virtual void setX(int& someNum) = 0; 

	virtual int getX() = 0;

	virtual int getY() = 0;

	virtual sf::RectangleShape getEnt() = 0;

};



class Paddle: public Entities {
	//inheritence is convinient
private:
	sf::RectangleShape paddle;
	int x_axis, y_axis;
public:

	Paddle() {
		y_axis = Screen_Height - 40;
		sf::Vector2f sizeRect = { 190,20 };
		paddle.setFillColor(sf::Color::White);
		paddle.setSize(sizeRect);
		x_axis = (Screen_Width / 2) - 120;
		y_axis = Screen_Height - 40;
		paddle.setPosition(x_axis, y_axis);
	}
	void setX(int& someNum) {
		//allows for position to be altered. Might remove since it was initially mearnt to work with an update function
		if (someNum + 120 > Screen_Width) {
			x_axis = x_axis;
		}
		else {
			x_axis = someNum;
		}
	}

	int getX() override{
		return x_axis;
	}

	int getY() override {
		return y_axis;
	}

	sf::RectangleShape getEnt() override {
		return paddle;
	}

	void Move(float velocityX, float deltaTime) {
		// Adjust paddle position based on velocity
		x_axis += velocityX * deltaTime;

		
		if (x_axis < 1) {
			x_axis = 1;
		}
		if (x_axis > (Screen_Width - paddle.getGlobalBounds().width - 1)) {
			x_axis = Screen_Width - paddle.getGlobalBounds().width - 1;
		}

		paddle.setPosition(x_axis, y_axis);
	}


};
class Ball {
private:
	int x_axis, y_axis, x_speed, y_speed;
	sf::CircleShape circle;
public:
	Ball() {
		x_axis = Screen_Width / 2;
		y_axis = Screen_Height / 2;
		circle.setRadius(20);
		circle.setPosition(x_axis, y_axis);
		circle.setFillColor(sf::Color::White);
		sf::Vector2f POO = { 20,20 }; //Point of origin
		circle.setOrigin(POO);

		x_speed = 12;
		y_speed = 12;
	}

	inline bool DetectCollision(Entities& paddle) {
		sf::IntRect paddleColBox(paddle.getEnt().getPosition().x, paddle.getEnt().getPosition().y,
			paddle.getEnt().getGlobalBounds().width, paddle.getEnt().getGlobalBounds().height);

		sf::IntRect ballColBox(circle.getPosition().x, circle.getPosition().y,
			circle.getGlobalBounds().width, circle.getGlobalBounds().height);

		if (paddleColBox.intersects(ballColBox)) {
			return true;
		}
		return false;
	}

	inline void avertPath(Entities& brick, float deltaTime = 1.0f) {
		
	}

	inline void Move(Entities& entity, float deltaTime = 1.0f) {
		if (circle.getPosition().x + 40 > Screen_Width || circle.getPosition().x <= 0) {
			x_speed *= -1;
			x_axis += deltaTime * x_speed;
		}
		else {
			x_axis += deltaTime * x_speed;
		}

		if (DetectCollision(entity) || circle.getPosition().y >= Screen_Height || circle.getPosition().y <= 0) {
			y_speed *= -1;
			y_axis += deltaTime * y_speed;
		}
		else {
			y_axis += deltaTime * y_speed;
		}

		circle.setPosition(x_axis, y_axis);
	}
	inline void setCoordinate(int x, int y) {
		y_speed *= y;
	}

	inline sf::CircleShape retBall() {
		return circle;
	}

};

class Bricks : public Entities {
private:
	sf::RectangleShape Brick;
	std::vector<sf::Color> colors = {
		sf::Color::Blue,
		sf::Color::Red,
		sf::Color::Yellow,
		sf::Color::Green,
		sf::Color::Magenta,
		sf::Color::Cyan,
	};
	int x_axis, y_axis;
	bool Spawn;
public:

	Bricks(int x, int y) : x_axis(x), y_axis(y) {
		Brick.setSize(sf::Vector2f{ 72 * 3,20 });
		Brick.setPosition(x_axis, y_axis);
		Brick.setOutlineColor(sf::Color::Black);
		Brick.setOutlineThickness(1);
		Spawn = true;
		
	}
	void setX(int& someNum) {
		x_axis = someNum;
	}

	inline bool DetectCollision(Ball& ball) {
		sf::IntRect ballColBox(ball.retBall().getPosition().x, ball.retBall().getPosition().y,
			ball.retBall().getGlobalBounds().width, ball.retBall().getGlobalBounds().height);

		sf::IntRect brickColBox(Brick.getPosition().x, Brick.getPosition().y,
			Brick.getGlobalBounds().width, Brick.getGlobalBounds().height);

		if (brickColBox.intersects(ballColBox)) {
			Spawn = false;
			ball.setCoordinate(-1, -1);
			return true;
		}
			return false;
	}

	inline bool getSpawn() {
		return Spawn;
	}

	void setSpawn(bool value) {
		Spawn = value;
	}


	int getX() {
		return x_axis;
	}

	int getY() {
		return y_axis;
	}

	sf::RectangleShape getEnt() {
		return Brick;
	}

	void setColor() {
		
		int index = 0 + (rand() % (colors.size() % (colors.size() - 0 + 1))); //int randomNumber = min + (rand() % (max - min + 1));
		Brick.setFillColor(colors[index]);
	}
};



Ball ball;
Paddle player;
std::vector<Bricks> bricks;
int x = 0, y = 0;


int main() {
	sf::RenderWindow window(sf::VideoMode(Screen_Width, Screen_Height), "Brick Breaker");
	window.setFramerateLimit(60);
	sf::Clock _clock;
	float previousTime = _clock.getElapsedTime().asSeconds();
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	//represent position of bricks
	int x_brick = 0, y_brick = 0;

	//set up bricks for spawns
	const int brickWidth = 72 *3;
	const int brickHeight = 20;
	const int bricksPerRow = Screen_Width / brickWidth;
	for (int i = 0; i <= 35 * 2; ++i) {
		Bricks someBrick(x_brick, y_brick);
		someBrick.setColor();
		bricks.push_back(someBrick);

		x_brick += brickWidth;
		if (x_brick >= Screen_Width) {
			x_brick = 0;
			y_brick += brickHeight;
		}
	}

	while (window.isOpen()) {
		float currentTime = _clock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - previousTime;
		sf::Event event;

		// Handle events
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}


		sf::Vector2f velocity(0.0f, 0.0f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			velocity.x = -860.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			velocity.x = 860.0f;
		}
		player.Move(velocity.x, deltaTime);

		ball.Move(player);
		window.clear(sf::Color::Black);
		
		// Inside your main loop:
		for (int i = 0; i < bricks.size(); ++i) {
			bricks[i].setSpawn(bricks[i].DetectCollision(ball));
		}

		for (int i = 0; i < bricks.size(); ++i) {
			if (!bricks[i].getSpawn()) {
				window.draw(bricks[i].getEnt());
				ball.avertPath(bricks.at(i));
			}
			else {
				bricks.erase(bricks.begin() + i);
				--i;
			}
		}

		window.draw(ball.retBall());
		window.draw(player.getEnt());
		window.display();

		previousTime = currentTime;
	}



	return 0;
}