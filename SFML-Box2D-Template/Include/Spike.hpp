#ifndef SPIKE_HPP
#define SPIKE_HPP

#include "Entity.hpp"

class Game;
class Spike : public Entity 
{
public:
	Spike(const sf::Vector2f& position, Game* game, const std::string& direction = "");

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;
};

#endif