#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Message.hpp"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Game;
class Entity
{
public:
	Entity(const sf::Vector2f& position, Game* game, const std::string& id);
	virtual ~Entity();

	virtual void handleInput() = 0;
	virtual void update(sf::Time delta) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual void handleMessage(Message& message) = 0;

	bool shouldRemove() const { return shouldRemove_; }
	b2Body* getBody() { return body_; }
	std::string getId() const { return id_; }
	sf::Sprite& getSprite() { return sprite_; }
	float getX() const { return getPosition().x; }
	float getY() const { return getPosition().y; }
	const sf::Sprite& getSprite() const { return sprite_; }
	float getWidth() const { return sprite_.getGlobalBounds().width; }
	float getHeight() const { return sprite_.getGlobalBounds().height; }
	sf::Vector2f getPosition() const { return sprite_.getPosition(); }
	sf::FloatRect getLocalBounds() const { return sprite_.getLocalBounds(); }
	sf::FloatRect getGlobalBounds() const { return sprite_.getGlobalBounds(); }

protected:
	Game* game_;
	bool paused_;
	b2Body* body_;
	std::string id_;
	sf::Sprite sprite_;
	bool shouldRemove_;
	sf::Time timeToPause_;
	sf::Clock pauseClock_;
	sf::Time pauseStartedTime_;
};

#endif