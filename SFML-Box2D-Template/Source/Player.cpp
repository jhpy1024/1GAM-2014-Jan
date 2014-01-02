#include "../Include/Player.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Game.hpp"

Player::Player(const sf::Vector2f& position, Game* game)
	: Entity(position, game)
	, Speed(1.f)
	, width_(70)
	, height_(94)
	, currFrame_(0)
	, numFrames_(11)
	, direction_(Direction::Right)
{
	spriteSheet_.loadFromFile("Assets/player.png");
	sprite_.setTexture(spriteSheet_);
	sprite_.setTextureRect(sf::IntRect(0, 0, width_, height_));
	sprite_.setOrigin(sprite_.getLocalBounds().left + sprite_.getLocalBounds().width / 2.f, 
		sprite_.getLocalBounds().top + sprite_.getLocalBounds().height / 2.f);

	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(position.x), pixelsToMeters(position.y));
	bodyDef.type = b2_dynamicBody;

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(width_ / 2.f), pixelsToMeters(height_ / 2.f));

	body_ = game->getWorld()->CreateBody(&bodyDef);
	body_->CreateFixture(&shape, 1.f);
}

void Player::handleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		body_->ApplyLinearImpulse(b2Vec2(-Speed, 0), body_->GetWorldCenter(), true);
		direction_ = Direction::Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		direction_ = Direction::Right;
		body_->ApplyLinearImpulse(b2Vec2(Speed, 0), body_->GetWorldCenter(), true);
	}
}

void Player::update(sf::Time delta)
{
	updateAnimation();

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
}

void Player::render(sf::RenderWindow& window)
{
	switch (direction_)
	{
	case Direction::Left:
		sprite_.setTextureRect(sf::IntRect(sprite_.getTextureRect().left, sprite_.getTextureRect().top, -width_, height_));
		break;
	case Direction::Right:
		sprite_.setTextureRect(sf::IntRect(sprite_.getTextureRect().left, sprite_.getTextureRect().top, width_, height_));
	}

	window.draw(sprite_);
}

void Player::updateAnimation()
{
	if (clock_.getElapsedTime() >= sf::milliseconds(50))
    {
		++currFrame_;

		if (currFrame_ == numFrames_)
			currFrame_ = 0;

		sprite_.setTextureRect(sf::IntRect(width_ * currFrame_, 0, sprite_.getTextureRect().width, height_));

		clock_.restart();
	}
}