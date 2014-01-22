#ifndef SHURIKEN_HPP
#define SHURIKEN_HPP

#include "Entity.hpp"

class Game;
class Shuriken : public Entity
{
public:
	Shuriken(const sf::Vector2f& position, const sf::Vector2f& velocity, Game* game);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;

private:
	static int Id;
};

#endif