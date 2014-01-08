#include "../Include/Coin.hpp"
#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/GotCoinMessage.hpp"

int Coin::CoinId = 0;

Coin::Coin(const sf::Vector2f& position, Game* game)
	: Entity(position, game, "coin " + std::to_string(CoinId))
{
	++CoinId;

	sprite_.setTexture(game->getTextureManager().getTexture("coin"));
	sprite_.setTextureRect(sf::IntRect(0, 0, 35, 35));

	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(position.x), pixelsToMeters(position.y));

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(sprite_.getTexture()->getSize().x) / 2.f, pixelsToMeters(sprite_.getTexture()->getSize().y) / 2.f);

	body_ = game->getWorld()->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.isSensor = true;
	fixtureDef.userData = this;

	body_->CreateFixture(&fixtureDef);
	body_->SetGravityScale(0.f);
	body_->SetFixedRotation(true);

	sprite_.setPosition(metersToPixels(body_->GetPosition().x), metersToPixels(body_->GetPosition().y));
}

Coin::~Coin()
{
	game_->getWorld()->DestroyBody(body_);
}

void Coin::handleInput()
{

}

void Coin::update(sf::Time delta)
{
	
}

void Coin::render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

void Coin::handleMessage(Message& message)
{
	switch (message.getType())
	{
	case GotCoinMsg:
		{
			GotCoinMessage& msg = static_cast<GotCoinMessage&>(message);
			if (msg.getCoinId() == id_)
				shouldRemove_ = true;
		}
		break;
	default:
		break;
	}
}

int Coin::getCoinId() const
{
	return CoinId;
}