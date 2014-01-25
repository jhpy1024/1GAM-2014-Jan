#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Finish.hpp"

Finish::Finish(const sf::Vector2f& position, Game* game)
	: Entity(position, game, "finish")
{
	sprite_.setTexture(game->getTextureManager().getTexture("spriteSheet"));
	sprite_.setTextureRect(sf::IntRect(0, 335, 32, 32));
	sprite_.setOrigin(sprite_.getLocalBounds().left + sprite_.getLocalBounds().width / 2.f, 
		sprite_.getLocalBounds().top + sprite_.getLocalBounds().height / 2.f);

	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(position.x), pixelsToMeters(position.y));

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(static_cast<float>(sprite_.getTexture()->getSize().x)) / 2.f,
		pixelsToMeters(static_cast<float>(sprite_.getTexture()->getSize().y)) / 2.f);

	body_ = game->getWorld()->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.isSensor = true;

	body_->CreateFixture(&fixtureDef);
	body_->SetGravityScale(0.f);
	body_->SetFixedRotation(true);
	body_->SetUserData(this);

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
}

void Finish::handleInput()
{

}

void Finish::update(sf::Time delta)
{

}

void Finish::render(sf::RenderWindow& window) 
{
	window.draw(sprite_);
}

void Finish::handleMessage(Message& message)
{

}