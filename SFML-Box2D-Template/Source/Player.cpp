#include "../Include/Player.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Game.hpp"
#include "../Include/GotCoinMessage.hpp"
#include "../Include/GetHealthMessage.hpp"
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
	, DefaultJumpSteps(6)
	, SuperJumpSteps(15)
	, jumpStepsLeft_(0)
	, numJumpSteps_(DefaultJumpSteps)
	, jumpPowerupTime_(sf::seconds(5.f))
	, superJump_(false)
	, footSensor_(game)
	, direction_(Direction::Right)
	, coins_(0)
	, health_(100)
	, HurtDelay(sf::seconds(0.2f))
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

	footBody_ = game->getWorld()->CreateBody(&footDef);
	footBody_->CreateFixture(&fixtureDef);
	footBody_->SetUserData(&footSensor_);
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && footSensor_.getNumContacts() >= 1)
		jumpStepsLeft_ = numJumpSteps_;
}

void Player::update(sf::Time)
{
	footBody_->SetTransform(b2Vec2(body_->GetPosition().x, body_->GetPosition().y + pixelsToMeters(height_ / 2.f)), footBody_->GetAngle());
	updateAnimation();

	if (superJump_)
	{
		if (superJumpClock_.getElapsedTime() - timeGotJumpPowerup_ >= jumpPowerupTime_)
		{
			superJump_ = false;
			numJumpSteps_ = DefaultJumpSteps;
		}
		else
		{
			numJumpSteps_ = SuperJumpSteps;
		}
	}

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
			coins_ += msg.getCoin().getWorth();
		}
		break;
	case GetAmountCoinsMsg:
		{
			GetAmountCoinsMessage& msg = static_cast<GetAmountCoinsMessage&>(message);
			msg.setCoins(coins_);
		}
		break;
	case HitSpikeMsg:
		{
			if (hurtClock_.getElapsedTime() >= HurtDelay)
			{
				health_ -= 25;
				hurtClock_.restart();
			}
		}
		break;
	case GetHealthMsg:
		{
			GetHealthMessage& msg = static_cast<GetHealthMessage&>(message);
			msg.setHealth(health_);
		}
		break;
	case GotJumpPowerupMsg:
		{
			timeGotJumpPowerup_ = superJumpClock_.getElapsedTime();
			superJump_ = true;
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