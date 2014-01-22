#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Shuriken.hpp"
#include "../Include/MonkeyEnemy.hpp"
#include "../Include/GetPositionMessage.hpp"

int MonkeyEnemy::Id = 0;

MonkeyEnemy::MonkeyEnemy(const sf::Vector2f& position, Game* game, const std::string& direction, float fireAngle)
	: Entity(position, game, "monkeyEnemy " + std::to_string(Id))
	, Width(64)
	, Height(75)
	, FireDelay(sf::seconds(1.f + (std::rand() % 3)))
	, FireRange(550.f)
	, fireAngle_(fireAngle)
	, ShurikenSpeed(0.5f)
{
	++Id;

	sprite_.setTexture(game->getTextureManager().getTexture("spriteSheet"));
	sprite_.setTextureRect(sf::IntRect(0, 230, Width, Height));
	sprite_.setOrigin(sprite_.getLocalBounds().left + sprite_.getLocalBounds().width / 2.f, 
		sprite_.getLocalBounds().top + sprite_.getLocalBounds().height / 2.f);

	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(position.x), pixelsToMeters(position.y));

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(static_cast<float>(Width * 0.5f)) / 2.f, pixelsToMeters(static_cast<float>(Height * 0.5f)) / 2.f);

	body_ = game->getWorld()->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.f;

	body_->CreateFixture(&fixtureDef);
	body_->SetFixedRotation(true);
	body_->SetUserData(this);

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));

	if (direction == "left")
	{
		sprite_.setScale(1.f, 1.f);
		direction_ = Direction::Left;
	}
	else if (direction == "right")
	{
		sprite_.setScale(-1.f, 1.f);
		direction_ = Direction::Right;
	}
}

void MonkeyEnemy::handleInput()
{

}

void MonkeyEnemy::update(sf::Time delta)
{
	if (fireClock_.getElapsedTime() >= FireDelay && inRangeOfPlayer())
	{
		fireShuriken();
		fireClock_.restart();
	}
}

void MonkeyEnemy::fireShuriken()
{
	float rotationRads = (fireAngle_ - 90.f) * (3.14159f / 180.f);
	game_->addEntity(new Shuriken(sprite_.getPosition(), sf::Vector2f(ShurikenSpeed * std::cos(rotationRads), ShurikenSpeed * std::sin(rotationRads)), game_));
}

bool MonkeyEnemy::inRangeOfPlayer()
{
	GetPositionMessage msg("player");
	game_->sendMessage(msg);

	float distToPlayerSquared = std::pow((sprite_.getPosition().x - msg.getPosition().x), 2) + std::pow((sprite_.getPosition().y - msg.getPosition().y), 2);

	return std::abs(distToPlayerSquared) <= std::pow(FireRange, 2);
}

void MonkeyEnemy::render(sf::RenderWindow& window) 
{
	window.draw(sprite_);
}

void MonkeyEnemy::handleMessage(Message& message)
{

}