#include "../Include/Entity.hpp"
#include "../Include/Game.hpp"

Entity::Entity(const sf::Vector2f& position, Game* game, const std::string& id)
	: game_(game)
	, id_(id)
	, shouldRemove_(false)
	, paused_(false)
{
	sprite_.setPosition(position);
}

Entity::~Entity()
{
	if (game_->getWorld() != nullptr)
		game_->getWorld()->DestroyBody(body_);
}

void Entity::remove()
{
	shouldRemove_ = true;
}