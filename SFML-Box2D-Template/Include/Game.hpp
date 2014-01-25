#ifndef GAME_HPP
#define GAME_HPP


#include <string>
#include <vector>
#include <memory>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Animation.hpp>

#include "Entity.hpp"
#include "Message.hpp"
#include "tmx/MapLoader.h"
#include "TextureManager.hpp"

enum GameState
{
	Play,
	Menu,
	Win,
	Lose,
};

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

	void addEntity(Entity* entity);
	void finish();

private:
	void createEntities();
	void createWorld();
	void loadTextures();

	void setState(GameState state);
	void initPlay();
	void initMenu();
	void initWin();
	void initLose();

	void handleInputPlay();
	void handleInputMenu();
	void handleInputWin();
	void handleInputLose();

	void updatePlay(sf::Time delta);
	void updateMenu(sf::Time delta);
	void updateWin(sf::Time delta);
	void updateLose(sf::Time delta);

	void renderPlay();
	void renderMenu();
	void renderWin();
	void renderLose();

	void handleInput();
	void update(sf::Time delta);
	void render();

	void updateView();
	void handleMessage(Message& message);

	void reset();

	void createBloodParticleSystem();
	void createSmokeParticleSystem();
	thor::UniversalEmitter createBloodEmitter();
	thor::UniversalEmitter createSmokeEmitter(const sf::Vector2f& position);

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

	sf::RectangleShape bgShape_;

	thor::ParticleSystem bloodParticleSystem_;
	thor::ParticleSystem smokeParticleSystem_;
	sf::Time timePlayerHitSpikes_;

	std::vector<Entity*> entitiesToAdd_;

	const int HealthBarScale;

	float rot;

	GameState state_;

	bool finished_;
	sf::Text finishedText_;
};

#endif