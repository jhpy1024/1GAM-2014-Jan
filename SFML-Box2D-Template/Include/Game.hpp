#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <Box2D/Box2D.h>

#include "Entity.hpp"
#include "Message.hpp"
#include "tmx/MapLoader.h"
#include "TextureManager.hpp"

class Game
{
public:
	Game();
	~Game();

	void run();
	void sendMessage(Message& message);

	b2World* getWorld() { return world_; }
	int getWidth() const { return Width;  }
	int getHeight() const { return Height; }
	TextureManager& getTextureManager() { return textureManager_; }
	std::vector<std::unique_ptr<Entity>>& getEntities() { return entities_; }

private:
	void createEntities();
	void createWorld();
	void loadTextures();

	void handleInput();
	void update(sf::Time delta);
	void render();

	void updateView();
	void handleMessage(Message& message);

	void reset();

private:
	const int Width;
	const int Height;
	const std::string Title;
	const sf::Time TimePerFrame;
	const b2Vec2 Gravity;

	b2World* world_;

	sf::View view_;
	sf::RenderWindow window_;

	std::vector<std::unique_ptr<Entity>> entities_;

	tmx::MapLoader mapLoader_;

	TextureManager textureManager_;

	sf::Font coinsFont_;
	sf::Text coinsText_;
	sf::Sprite healthBar_;

	sf::Vector2f playerStartPos_;

	bool hasFocus_;
	bool shouldReset_;
	sf::Clock clock_;

	sf::Sprite bgSprite_;
};

#endif