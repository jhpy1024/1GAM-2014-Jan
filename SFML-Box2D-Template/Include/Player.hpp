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
	void shootBullet();
	bool fireDelayFinished();

private:
	const float Speed;
	int width_;
	int height_;
	const float ShapeDecreaseFactor;

	const int DefaultJumpSteps;
	const int SuperJumpSteps;
	int jumpStepsLeft_;
	int numJumpSteps_;
	sf::Clock superJumpClock_;
	sf::Time timeGotJumpPowerup_;
	sf::Time jumpPowerupTime_;
	const float JumpForceFactor;
	bool superJump_;

	b2Body* footBody_;
	b2Fixture* footFixture_;
	PlayerFootSensor footSensor_;	

	enum class Direction { Left, Right };
	Direction direction_;

	int coins_;
	int health_;
	sf::Clock hurtClock_;
	const sf::Time HurtDelay;
	const float SpikeHealthDecrease;
	const float ShurikenHealthDecrease;
	const float CannonBallHealthDecrease;

	sf::Clock fireClock_;
	const sf::Time FireDelay;

	b2Vec2 velBeforePause_;
};

#endif