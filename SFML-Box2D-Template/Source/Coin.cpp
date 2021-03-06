#include "../Include/Coin.hpp"
#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/GotCoinMessage.hpp"

int Coin::CoinId = 0;

Coin::Coin(const sf::Vector2f& position, Game* game, int worth)
	: Entity(position, game, "coin " + std::to_string(CoinId))
	, worth_(worth)
	, TextureWidth(35)
	, TextureHeight(35)
{
	++CoinId;

	sprite_.setTexture(game->getTextureManager().getTexture("spriteSheet"));
	switch (worth)
	{
	case 1:
		sprite_.setTextureRect(sf::IntRect(128, 0, TextureWidth, TextureHeight));
		break;
	case 5:
		sprite_.setTextureRect(sf::IntRect(163, 0, TextureWidth, TextureHeight));
		break;
	case 100:
		sprite_.setTextureRect(sf::IntRect(198, 0, TextureWidth, TextureHeight));
		break;
	default:
		break;
	}
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