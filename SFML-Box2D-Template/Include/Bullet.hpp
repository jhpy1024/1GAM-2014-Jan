#ifndef BULLET_HPP
#define BULLET_HPP

#include "Entity.hpp"

class Game;
class Bullet : public Entity
{
public:
	Bullet(const sf::Vector2f& position, int direction, Game* game);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;

private:
	static int Id;

	const float Speed;
	const int TextureWidth;
	const int TextureHeight;
};

#endif