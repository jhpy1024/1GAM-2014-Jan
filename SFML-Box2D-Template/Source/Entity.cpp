#include "../Include/Entity.hpp"
#include "../Include/Game.hpp"

Entity::Entity(const sf::Vector2f& position, Game* game, const std::string& id)
	: game_(game)
	, id_(id)
{
	sprite_.setPosition(position);
}