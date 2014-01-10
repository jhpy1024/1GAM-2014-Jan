#ifndef GROUND_HPP
#define GROUND_HPP

#include "Entity.hpp"

class Game;
class Ground : public Entity
{
public:
	Ground(Game* game);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;
};

#endif