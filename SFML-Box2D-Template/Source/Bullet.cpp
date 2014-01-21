#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Bullet.hpp"

int Bullet::Id = 0;

Bullet::Bullet(const sf::Vector2f& position, int direction, Game* game)
	: Entity(position, game, "bullet " + std::to_string(Id))
	, Speed(0.5f)
	, TextureWidth(16)
	, TextureHeight(16)
{
	++Id;

	sprite_.setTexture(game->getTextureManager().getTexture("spriteSheet"));
	sprite_.setTextureRect(sf::IntRect(0, 0, TextureWidth, TextureHeight));
	sprite_.setOrigin(sprite_.getLocalBounds().left + sprite_.getLocalBounds().width / 2.f, 
		sprite_.getLocalBounds().top + sprite_.getLocalBounds().height / 2.f);

	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(position.x), pixelsToMeters(position.y));
	bodyDef.type = b2_dynamicBody;

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(static_cast<float>(TextureWidth)) / 2.f,
		pixelsToMeters(static_cast<float>(TextureHeight)) / 2.f);

	body_ = game->getWorld()->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.restitution = 0.f;
	fixtureDef.isSensor = true;

	body_->CreateFixture(&fixtureDef);
	body_->SetFixedRotation(true);
	body_->SetUserData(this);
	body_->SetGravityScale(0.f);
	body_->ApplyLinearImpulse(b2Vec2(Speed * direction, 0.f), body_->GetWorldCenter(), true);

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
}

void Bullet::handleInput()
{

}

void Bullet::update(sf::Time delta)
{
	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
}

void Bullet::render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

void Bullet::handleMessage(Message& message)
{

}