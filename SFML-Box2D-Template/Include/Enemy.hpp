#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

enum class Direction
{
	Left,
	Right,
};

class Game;
class Enemy : public Entity
{
public:
	Enemy(const sf::Vector2f& position, Game* game);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;
private:
	static int Id;

	Direction direction_;

	sf::Sprite cannonSprite_;

	const int Width;
	const int Height;
	const float ShapeDecreaseFactor;
	const float CannonRotationSpeed;
	const float CannonTextureWidth;
	const float CannonTextureHeight;
};

#endif