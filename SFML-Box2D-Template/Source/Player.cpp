#include "../Include/Player.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Game.hpp"
#include "../Include/GotCoinMessage.hpp"
#include "../Include/PlayerFootSensor.hpp"
#include "../Include/GetPositionMessage.hpp"
#include "../Include/SetPositionMessage.hpp"
#include "../Include/GetVelocityMessage.hpp"
#include "../Include/SetVelocityMessage.hpp"
#include "../Include/GetAmountCoinsMessage.hpp"

Player::Player(const sf::Vector2f& position, Game* game)
	: Entity(position, game, "player")
	, Speed(5.f)
	, width_(70)
	, height_(94)
	, currFrame_(0)
	, numFrames_(11)
	, jumpStepsLeft_(0)
	, footSensor_(game)
	, direction_(Direction::Right)
	, coins_(0)
{
	sprite_.setTexture(game->getTextureManager().getTexture("player"));
	sprite_.setTextureRect(sf::IntRect(0, 0, width_, height_));
	sprite_.setOrigin(sprite_.getLocalBounds().left + sprite_.getLocalBounds().width / 2.f, 
		sprite_.getLocalBounds().top + sprite_.getLocalBounds().height / 2.f);

	// -- Creating main body --
	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(position.x), pixelsToMeters(position.y));
	bodyDef.type = b2_dynamicBody;

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(width_ / 2.f), pixelsToMeters(height_ / 2.f));

	body_ = game->getWorld()->CreateBody(&bodyDef);
	body_->CreateFixture(&shape, 1.f);
	body_->SetFixedRotation(true);
	body_->SetLinearDamping(2.f);
	body_->SetUserData(this);

	// -- Creating foot sensor --
	b2BodyDef footDef;
	footDef.position.Set(bodyDef.position.x, bodyDef.position.y + pixelsToMeters(height_ / 2.f));
	footDef.type = b2_dynamicBody;
	
	shape.SetAsBox(pixelsToMeters(width_ / 10.f), pixelsToMeters(height_ / 15.f));
	b2FixtureDef fixtureDef;
	fixtureDef.isSensor = true;
	fixtureDef.shape = &shape;
	fixtureDef.userData = &footSensor_;

	footBody_ = game->getWorld()->CreateBody(&footDef);
	footBody_->CreateFixture(&fixtureDef);

	sensorShape_.setSize(sf::Vector2f(width_ / 5.f, height_ / 7.5f));
	sensorShape_.setOrigin(sensorShape_.getLocalBounds().left + sensorShape_.getLocalBounds().width / 2.f, 
		sensorShape_.getLocalBounds().top + sensorShape_.getLocalBounds().height / 2.f);
}

Player::~Player()
{
	game_->getWorld()->DestroyBody(body_);
}

void Player::handleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		body_->SetLinearVelocity(b2Vec2(-Speed, body_->GetLinearVelocity().y));
		direction_ = Direction::Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		body_->SetLinearVelocity(b2Vec2(Speed, body_->GetLinearVelocity().y));
		direction_ = Direction::Right;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && footSensor_.getNumContacts() >= 2)
		jumpStepsLeft_ = 6;
}

void Player::update(sf::Time delta)
{
	footBody_->SetTransform(b2Vec2(body_->GetPosition().x, body_->GetPosition().y + pixelsToMeters(height_ / 2.f)), footBody_->GetAngle());
	sensorShape_.setPosition(metersToPixels(footBody_->GetPosition().x), metersToPixels(footBody_->GetPosition().y));
	updateAnimation();

	if (jumpStepsLeft_ > 0)
	{
		body_->ApplyForceToCenter(b2Vec2(0.f, -Speed * 5), true);
		--jumpStepsLeft_;
	}

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
	window.draw(sensorShape_);
}

void Player::handleMessage(Message& message)
{
	switch (message.getType())
	{
	case GetPositionMsg:
		{
			GetPositionMessage& msg = static_cast<GetPositionMessage&>(message);
			msg.setPosition(getPosition());
		}
		break;
	case SetPositionMsg:
		{
			SetPositionMessage& msg = static_cast<SetPositionMessage&>(message);
			sprite_.setPosition(msg.getPosition());
			body_->SetTransform(b2Vec2(pixelsToMeters(msg.getPosition().x), pixelsToMeters(msg.getPosition().y)), body_->GetAngle());
		}
		break;
	case GetVelocityMsg:
		{	
			GetVelocityMessage& msg = static_cast<GetVelocityMessage&>(message);
			msg.setVelocity(body_->GetLinearVelocity().x, body_->GetLinearVelocity().y);
		};
		break;
	case SetVelocityMsg:
		{
			SetVelocityMessage& msg = static_cast<SetVelocityMessage&>(message);
			body_->SetLinearVelocity(b2Vec2(msg.getVelocity().x, msg.getVelocity().y));
		}
		break;
	case GotCoinMsg:
		{
			GotCoinMessage& msg = static_cast<GotCoinMessage&>(message);
			coins_ += 10;
		}
		break;
	case GetAmountCoinsMsg:
		{
			GetAmountCoinsMessage& msg = static_cast<GetAmountCoinsMessage&>(message);
			msg.setCoins(coins_);
		}
		break;
	default:
		break;
	}
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