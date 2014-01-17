#ifndef JUMP_POWERUP_HPP
#define JUMP_POWERUP_HPP

#include "Entity.hpp"

class Game;
class JumpPowerup : public Entity
{
public:
	JumpPowerup(const sf::Vector2f& position, Game* game);

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