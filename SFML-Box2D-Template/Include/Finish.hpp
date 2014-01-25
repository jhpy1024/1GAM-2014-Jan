#ifndef FINISH_HPP
#define FINISH_HPP

#include "Entity.hpp"

class Game;
class Finish : public Entity
{
public:
	Finish(const sf::Vector2f& position, Game* game);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;
};

#endif