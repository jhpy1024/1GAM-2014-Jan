#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Spike.hpp"

Spike::Spike(const sf::Vector2f& position, Game* game)
	: Entity(position, game, "spike")
{
	sprite_.setTexture(game->getTextureManager().getTexture("spike"));
	sprite_.setTextureRect(sf::IntRect(0, 0, 32, 32));

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

Spike::~Spike()
{
	game_->getWorld()->DestroyBody(body_);
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