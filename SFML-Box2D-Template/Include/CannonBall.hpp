#ifndef CANNON_BALL_HPP
#define CANNON_BALL_HPP

#include "Entity.hpp"

class Game;
class CannonBall : public Entity
{
public:
	CannonBall(const sf::Vector2f& position, const sf::Vector2f& velocity, Game* game);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;

private:
	static int Id;

	const int TextureWidth;
	const int TextureHeight;
};

#endif