#include "../Include/Game.hpp"
#include "../Include/Enemy.hpp"
#include "../Include/Utils.hpp"
#include "../Include/CannonBall.hpp"
#include "../Include/ShotEnemyMessage.hpp"
#include "../Include/GetPositionMessage.hpp"
#include "../Include/GetRotationMessage.hpp"

int Enemy::Id = 0;

Enemy::Enemy(const sf::Vector2f& position, Game* game)
	: Entity(position, game, "enemy " + std::to_string(Id))
	, direction_(Direction::Left)
	, Width(64)
	, Height(64)
	, ShapeDecreaseFactor(0.75f)
	, CannonRotationSpeed(1.f)
	, CannonTextureWidth(32.f)
	, CannonTextureHeight(32.f)
	, FireDelay(sf::seconds(0.7f))
	, FireSpeed(0.5f)
	, ShootRange(550.f)
	, health_(100)
	, BulletHealthDecrease(25)
{
	++Id;
	
	sprite_.setTexture(game->getTextureManager().getTexture("spriteSheet"));
	sprite_.setTextureRect(sf::IntRect(64, 0, Width, Height));
	sprite_.setOrigin(sprite_.getLocalBounds().left + sprite_.getLocalBounds().width / 2.f, 
		sprite_.getLocalBounds().top + sprite_.getLocalBounds().height / 2.f);

	cannonSprite_.setTexture(game->getTextureManager().getTexture("spriteSheet"));
	cannonSprite_.setTextureRect(sf::IntRect(32, 0, CannonTextureWidth, CannonTextureHeight));
	cannonSprite_.setOrigin(cannonSprite_.getLocalBounds().left + cannonSprite_.getLocalBounds().width / 2.f,
		cannonSprite_.getLocalBounds().top + cannonSprite_.getLocalBounds().height);
	cannonSprite_.setPosition(sprite_.getPosition());

	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(position.x), pixelsToMeters(position.y));

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(static_cast<float>(Width * ShapeDecreaseFactor)) / 2.f, pixelsToMeters(static_cast<float>(Height * ShapeDecreaseFactor)) / 2.f);

	body_ = game->getWorld()->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.f;

	body_->CreateFixture(&fixtureDef);
	body_->SetFixedRotation(true);
	body_->SetUserData(this);

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));

	healthBar_.setTexture(game->getTextureManager().getTexture("spriteSheet"));
	healthBar_.setTextureRect(sf::IntRect(0, 100, 100, 15));
	healthBar_.setOrigin(healthBar_.getLocalBounds().left + healthBar_.getLocalBounds().width / 2.f, 
		healthBar_.getLocalBounds().top + healthBar_.getLocalBounds().height / 2.f);
	healthBar_.setPosition(sprite_.getPosition().x, sprite_.getPosition().y - sprite_.getLocalBounds().height / 2.f);
}

void Enemy::handleInput()
{

}

void Enemy::update(sf::Time delta)
{
	cannonSprite_.rotate(CannonRotationSpeed);
	
	if (fireClock_.getElapsedTime() >= FireDelay && appropriateCannonAngle() && inRangeOfPlayer())
	{
		fireCannon();
		fireClock_.restart();
	}
}

void Enemy::fireCannon()
{
	float rotationRads = (cannonSprite_.getRotation() - 90.f) * (3.14159f / 180.f);
	game_->addEntity(new CannonBall(
		sprite_.getPosition(), sf::Vector2f(FireSpeed * std::cos(rotationRads), FireSpeed * std::sin(rotationRads)), game_));
}

bool Enemy::inRangeOfPlayer() const
{
	GetPositionMessage msg("player");
	game_->sendMessage(msg);

	float distToPlayerSquared = std::pow((sprite_.getPosition().x - msg.getPosition().x), 2) + std::pow((sprite_.getPosition().y - msg.getPosition().y), 2);

	return std::abs(distToPlayerSquared) <= std::pow(ShootRange, 2);
}

bool Enemy::appropriateCannonAngle() const
{
	return (cannonSprite_.getRotation() >= 0.f && cannonSprite_.getRotation() <= 90.f) 
		|| (cannonSprite_.getRotation() >= 270.f && cannonSprite_.getRotation() <= 360.f);
}

void Enemy::render(sf::RenderWindow& window)
{
	switch (direction_)
	{
	case Direction::Left:
		sprite_.setScale(1.f, 1.f);
		break;
	case Direction::Right:
		sprite_.setScale(-1.f, 1.f);
		break;
	default:
		break;
	}

	window.draw(sprite_);
	window.draw(healthBar_);
	window.draw(cannonSprite_);
}

void Enemy::hitByBullet()
{
	health_ -= BulletHealthDecrease;

	// Have we died?
	if (health_ <= 0)
		shouldRemove_ = true;

	healthBar_.setTextureRect(sf::IntRect(0, 100, health_, healthBar_.getTextureRect().height));
	healthBar_.setOrigin(healthBar_.getLocalBounds().left + healthBar_.getLocalBounds().width / 2.f,
				healthBar_.getLocalBounds().top + healthBar_.getLocalBounds().height / 2.f);
	healthBar_.setPosition(sprite_.getPosition().x, sprite_.getPosition().y - sprite_.getLocalBounds().height / 2.f);
}

void Enemy::handleMessage(Message& message)
{
	switch (message.getType())
	{
	case GetRotationMsg:
		{
			auto& msg = static_cast<GetRotationMessage&>(message);
			msg.setRotation(cannonSprite_.getRotation());
		}
		break;
	case ShotEnemyMsg:
		{
			auto& msg = static_cast<ShotEnemyMessage&>(message);
			if (msg.getEnemy().getId() == id_)
				hitByBullet();
		}
		break;
	default:
		break;
	}
}