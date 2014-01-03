#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <Box2D/Box2D.h>

#include "Entity.hpp"
#include "tmx/MapLoader.h"

class Game
{
public:
	Game();

	void run();

	b2World* getWorld() { return world_; }
	int getWidth() const { return Width;  }
	int getHeight() const { return Height; }
	std::vector<std::unique_ptr<Entity>>& getEntities() { return entities_; }

private:
	void handleInput();
	void update(sf::Time delta);
	void render();

private:
	const int Width;
	const int Height;
	const std::string Title;
	const sf::Time TimePerFrame;
	const b2Vec2 Gravity;

	b2World* world_;
	sf::RenderWindow window_;
	std::vector<std::unique_ptr<Entity>> entities_;

	b2Body* groundBody_;
	sf::RectangleShape groundShape_;

	tmx::MapLoader mapLoader_;
};

#endif