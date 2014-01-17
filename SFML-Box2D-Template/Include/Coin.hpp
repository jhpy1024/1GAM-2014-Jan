#ifndef COIN_HPP
#define COIN_HPP

#include "Entity.hpp"

class Game;
class Coin : public Entity
{
public:
	Coin(const sf::Vector2f& position, Game* game, int worth);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;

	int getCoinId() const;
	int getWorth() const { return worth_; }

private:
	int worth_;
	static int CoinId;

	const int TextureWidth;
	const int TextureHeight;
};

#endif