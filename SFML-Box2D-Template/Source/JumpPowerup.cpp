#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/JumpPowerup.hpp"
#include "../Include/GotJumpPowerupMessage.hpp"

int JumpPowerup::Id = 0;

JumpPowerup::JumpPowerup(const sf::Vector2f& position, Game* game)
	: Entity(position, game, "jumpPowerup " + std::to_string(Id))
{
	++Id;

	sprite_.setTexture(game->getTextureManager().getTexture("jumpPowerup"));
	sprite_.setTextureRect(sf::IntRect(0, 0, 32, 32));
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

void JumpPowerup::handleInput()
{

}

void JumpPowerup::update(sf::Time delta)
{

}

void JumpPowerup::render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

void JumpPowerup::handleMessage(Message& message)
{
	switch (message.getType())
	{
	case GotJumpPowerupMsg:
		{
			auto msg = static_cast<GotJumpPowerupMessage&>(message);
			if (msg.getPowerup().getId() == id_)
				shouldRemove_ = true;
		}
		break;
	}
}