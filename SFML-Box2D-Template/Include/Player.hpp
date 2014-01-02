#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

class Player : public Entity
{
public:
	Player(const sf::Vector2f& position, Game* game);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;

private:
	void updateAnimation();

private:
	const float Speed;
	int width_;
	int height_;
	int currFrame_;
	int numFrames_;
	sf::Texture spriteSheet_;
	sf::Clock clock_;

	enum class Direction { Left, Right };
	Direction direction_;
};

#endif