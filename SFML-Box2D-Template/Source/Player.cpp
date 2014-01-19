#include "../Include/Player.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Game.hpp"
#include "../Include/Bullet.hpp"
#include "../Include/ResumedMessage.hpp"
#include "../Include/GotCoinMessage.hpp"
#include "../Include/GetHealthMessage.hpp"
#include "../Include/PlayerFootSensor.hpp"
#include "../Include/GetPositionMessage.hpp"
#include "../Include/PauseEntityMessage.hpp"
#include "../Include/SetPositionMessage.hpp"
#include "../Include/GetVelocityMessage.hpp"
#include "../Include/SetVelocityMessage.hpp"
#include "../Include/GetAmountCoinsMessage.hpp"

Player::Player(const sf::Vector2f& position, Game* game)
	: Entity(position, game, "player")
	, Speed(5.f)
	, width_(70)
	, height_(94)
	, ShapeDecreaseFactor(0.1f)
	, DefaultJumpSteps(6)
	, SuperJumpSteps(15)
	, jumpStepsLeft_(0)
	, numJumpSteps_(DefaultJumpSteps)
	, jumpPowerupTime_(sf::seconds(5.f))
	, JumpForceFactor(5.f)
	, superJump_(false)
	, footSensor_(game)
	, direction_(Direction::Right)
	, coins_(0)
	, health_(100)
	, HurtDelay(sf::seconds(0.2f))
	, SpikeHealthDecrease(25.f)
	, CannonBallHealthDecrease(SpikeHealthDecrease)
	, FireDelay(sf::seconds(0.5f))
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
	body_->SetLinearDamping(3.f);
	body_->SetUserData(this);

	// -- Creating foot sensor --
	b2BodyDef footDef;
	footDef.position.Set(bodyDef.position.x, bodyDef.position.y + pixelsToMeters(height_ / 2.f));
	footDef.type = b2_dynamicBody;
	
	shape.SetAsBox(pixelsToMeters(width_ * ShapeDecreaseFactor), pixelsToMeters(height_ * ShapeDecreaseFactor));
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
		sprite_.setScale(-1.f, 1.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		body_->SetLinearVelocity(b2Vec2(Speed, body_->GetLinearVelocity().y));
		direction_ = Direction::Right;
		sprite_.setScale(1.f, 1.f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && footSensor_.getNumContacts() >= 1)
		jumpStepsLeft_ = numJumpSteps_;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && fireDelayFinished())
		shootBullet();
}

bool Player::fireDelayFinished()
{
	return fireClock_.getElapsedTime() >= FireDelay;
}

void Player::shootBullet()
{
	game_->addEntity(new Bullet(sprite_.getPosition(), (direction_ == Direction::Left ? -1 : 1), game_));
	fireClock_.restart();
}

void Player::update(sf::Time)
{
	if (paused_)
	{
		// Is pause time over?
		if (pauseClock_.getElapsedTime() - pauseStartedTime_ >= timeToPause_)
		{
			paused_ = false;
			body_->SetLinearVelocity(velBeforePause_);
			game_->sendMessage(ResumedMessage("game", ResumedFrom::Spikes));
		}
		else
		{
			// Still paused so don't update
			// Set the velocity to zero to make sure we don't move while paused.
			body_->SetLinearVelocity(b2Vec2_zero);
			return;
		}
	}

	// Set the position of the foot sensor to the correct position.
	footBody_->SetTransform(b2Vec2(body_->GetPosition().x, body_->GetPosition().y + pixelsToMeters(height_ / 2.f)), footBody_->GetAngle());

	if (superJump_)
	{
		// Is super jump time over?
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

	// If there are still jump steps left, apply the jump force.
	if (jumpStepsLeft_ > 0)
	{
		body_->ApplyForceToCenter(b2Vec2(0.f, -Speed * JumpForceFactor), true);
		--jumpStepsLeft_;
	}

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
}

void Player::render(sf::RenderWindow& window)
{
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
			if (paused_) break;
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
				health_ -= SpikeHealthDecrease;
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
	case PauseEntityMsg:
		{
			PauseEntityMessage& msg = static_cast<PauseEntityMessage&>(message);
			timeToPause_ = msg.getPauseTime();
			pauseStartedTime_ = pauseClock_.getElapsedTime();
			paused_ = true;

			velBeforePause_ = body_->GetLinearVelocity();
		}
		break;
	case HitCannonBallMsg:
		{
			health_ -= CannonBallHealthDecrease;
		}
		break;
	default:
		break;
	}
}