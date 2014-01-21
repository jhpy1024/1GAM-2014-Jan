#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/MonkeyEnemy.hpp"

int MonkeyEnemy::Id = 0;

MonkeyEnemy::MonkeyEnemy(const sf::Vector2f& position, Game* game, const std::string& direction)
	: Entity(position, game, "monkeyEnemy " + std::to_string(Id))
	, Width(64)
	, Height(75)
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

}

void MonkeyEnemy::render(sf::RenderWindow& window) 
{

	window.draw(sprite_);
}

void MonkeyEnemy::handleMessage(Message& message)
{

}