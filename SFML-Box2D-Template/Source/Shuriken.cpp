#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Shuriken.hpp"

int Shuriken::Id = 0;

Shuriken::Shuriken(const sf::Vector2f& position, const sf::Vector2f& velocity, Game* game)
	: Entity(position, game, "shuriken " + std::to_string(Id))
{
	++Id;

	sprite_.setTexture(game->getTextureManager().getTexture("spriteSheet"));
	sprite_.setTextureRect(sf::IntRect(0, 310, 20, 20));
	sprite_.setOrigin(sprite_.getLocalBounds().left + sprite_.getLocalBounds().width / 2.f, 
		sprite_.getLocalBounds().top + sprite_.getLocalBounds().height / 2.f);

	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(position.x), pixelsToMeters(position.y));
	bodyDef.type = b2_dynamicBody;

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(static_cast<float>(20)) / 2.f,
		pixelsToMeters(static_cast<float>(20)) / 2.f);

	body_ = game->getWorld()->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.restitution = 0.f;
	fixtureDef.isSensor = true;

	body_->CreateFixture(&fixtureDef);
	body_->SetFixedRotation(false);
	body_->SetUserData(this);
	body_->SetGravityScale(0.f);
	body_->ApplyLinearImpulse(b2Vec2(velocity.x, velocity.y), body_->GetWorldCenter(), true);
	body_->SetAngularVelocity(10.f);

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
}

void Shuriken::handleInput()
{

}

void Shuriken::update(sf::Time delta)
{
	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
	sprite_.setRotation(body_->GetAngle() * (180.f / 3.14159f));
}

void Shuriken::render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

void Shuriken::handleMessage(Message& message)
{

}