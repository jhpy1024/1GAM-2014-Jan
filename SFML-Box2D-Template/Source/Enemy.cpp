#include "../Include/Game.hpp"
#include "../Include/Enemy.hpp"
#include "../Include/Utils.hpp"

int Enemy::Id = 0;

Enemy::Enemy(const sf::Vector2f& position, Game* game)
	: Entity(position, game, "enemy " + std::to_string(Id))
	, direction_(Direction::Left)
	, width_(43)
	, height_(28)
{
	++Id;
	sprite_.setTexture(game->getTextureManager().getTexture("enemy"));
	sprite_.setTextureRect(sf::IntRect(0, 0, width_, height_));
	sprite_.setOrigin(sprite_.getLocalBounds().left + sprite_.getLocalBounds().width / 2.f, 
		sprite_.getLocalBounds().top + sprite_.getLocalBounds().height / 2.f);

	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(position.x), pixelsToMeters(position.y));

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(static_cast<float>(width_ * 0.75f)) / 2.f, pixelsToMeters(static_cast<float>(height_ * 0.75f)) / 2.f);

	body_ = game->getWorld()->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.f;

	body_->CreateFixture(&fixtureDef);
	body_->SetFixedRotation(true);
	body_->SetUserData(this);

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
}

void Enemy::handleInput()
{

}

void Enemy::update(sf::Time delta)
{
	
}

void Enemy::render(sf::RenderWindow& window)
{
	switch (direction_)
	{
	case Direction::Left:
		sprite_.setScale(1.f, 1.f);
		break;
	case Direction::Right:
		sprite_.setScale(-1.f, 1.f);
		break;
	default:
		break;
	}

	window.draw(sprite_);
}

void Enemy::handleMessage(Message& message)
{

}