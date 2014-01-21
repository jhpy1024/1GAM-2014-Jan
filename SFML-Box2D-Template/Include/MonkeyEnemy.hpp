#ifndef MONKEY_ENEMY_HPP
#define MONKEY_ENEMY_HPP

#include "Enemy.hpp"
#include "Entity.hpp"

class Game;
class MonkeyEnemy : public Entity
{
public:
	MonkeyEnemy(const sf::Vector2f& position, Game* game, const std::string& direction);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;

private:
	static int Id;

	const int Width;
	const int Height;

	Direction direction_;
};

#endif