#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Spike.hpp"

Spike::Spike(const sf::Vector2f& position, Game* game, const std::string& direction)
	: Entity(position, game, "spike")
	, TextureWidth(32)
	, TextureHeight(32)
{
	sprite_.setTexture(game_->getTextureManager().getTexture("spriteSheet"));
	if (direction == "down")
		sprite_.setTextureRect(sf::IntRect(0, 115, TextureWidth, TextureHeight));
	else if (direction == "left")
		sprite_.setTextureRect(sf::IntRect(32, 115, TextureWidth, TextureHeight));
	else if (direction == "")
		sprite_.setTextureRect(sf::IntRect(64, 115, TextureWidth, TextureHeight));
	else if (direction == "right")
		sprite_.setTextureRect(sf::IntRect(96, 115, TextureWidth, TextureHeight));

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

void Spike::handleInput() 
{

}

void Spike::update(sf::Time)
{

}

void Spike::render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

void Spike::handleMessage(Message&)
{

}