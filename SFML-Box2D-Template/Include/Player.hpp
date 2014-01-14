#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "PlayerFootSensor.hpp"

class Player : public Entity
{
public:
	Player(const sf::Vector2f& position, Game* game);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;

private:
	void updateAnimation();

private:
	const float Speed;
	int width_;
	int height_;
	int currFrame_;
	int numFrames_;
	sf::Clock clock_;

	int jumpStepsLeft_;

	b2Body* footBody_;
	b2Fixture* footFixture_;
	PlayerFootSensor footSensor_;	

	enum class Direction { Left, Right };
	Direction direction_;

	int coins_;
	int health_;
	sf::Clock hurtClock_;
	const sf::Time HurtDelay;
};

#endif