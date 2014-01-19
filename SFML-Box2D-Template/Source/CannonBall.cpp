#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/CannonBall.hpp"
#include "../Include/CannonFiredMessage.hpp"
#include "../Include/HitCannonBallMessage.hpp"

int CannonBall::Id = 0;

CannonBall::CannonBall(const sf::Vector2f& position, const sf::Vector2f& velocity, Game* game)
	: Entity(position, game, "cannonBall " + std::to_string(Id))
	, TextureWidth(16)
	, TextureHeight(16)
{
	++Id;

	sprite_.setTexture(game->getTextureManager().getTexture("cannonBall"));
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
	body_->ApplyLinearImpulse(b2Vec2(velocity.x, velocity.y), body_->GetWorldCenter(), true);

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));

	CannonFiredMessage msg("all", sprite_.getPosition());
	game->sendMessage(msg);
}

void CannonBall::handleInput() 
{

}

void CannonBall::update(sf::Time delta)
{
	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
}

void CannonBall::render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

void CannonBall::handleMessage(Message& message)
{
	switch (message.getType())
	{
	case HitCannonBallMsg:
		{
			auto msg = static_cast<HitCannonBallMessage&>(message);
			if (msg.getCannonBall().getId() == id_)
				shouldRemove_ = true;
		}
		break;
	}
}