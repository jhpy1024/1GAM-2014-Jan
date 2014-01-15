#include "../Include/Game.hpp"
#include "../Include/Coin.hpp"
#include "../Include/Spike.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Player.hpp"
#include "../Include/Ground.hpp"
#include "../Include/JumpPowerup.hpp"
#include "../Include/tmx/tmx2box2d.h"
#include "../Include/GotCoinMessage.hpp"
#include "../Include/ContactListener.hpp"
#include "../Include/GetHealthMessage.hpp"
#include "../Include/GetPositionMessage.hpp"
#include "../Include/SetPositionMessage.hpp"
#include "../Include/SetVelocityMessage.hpp"
#include "../Include/GetVelocityMessage.hpp"
#include "../Include/GetAmountCoinsMessage.hpp"
#include "../Include/GotJumpPowerupMessage.hpp"

Game::Game()
	: Width(1280)
	, Height(720)
	, Title("1GAM | Jan | 2014")
	, TimePerFrame(sf::seconds(1.f / 60.f))
	, Gravity(0.f, 10.f)
	, window_(sf::VideoMode(Width, Height), Title, sf::Style::Close)
	, world_(new b2World(Gravity))
	, mapLoader_("Assets/")
	, hasFocus_(true)
	, shouldReset_(false)
{
	createEntities();
	createWorld();
	loadTextures();

	coinsFont_.loadFromFile("Assets/coinsFont.ttf");
	coinsText_.setFont(coinsFont_);
	coinsText_.setColor(sf::Color::Red);
	coinsText_.setString("Coins: 0");
	coinsText_.setCharacterSize(30);
	coinsText_.setPosition(32.f, 30.f);

	healthBar_.setTexture(textureManager_.getTexture("healthBar"));
	healthBar_.setOrigin(healthBar_.getLocalBounds().left + healthBar_.getLocalBounds().width / 2.f,
		healthBar_.getLocalBounds().top + healthBar_.getLocalBounds().height / 2.f);
	healthBar_.setPosition(Width / 2.f, 50.f);
	
	view_.setCenter(Width / 2.f, Height / 2.f);
	view_.setSize(static_cast<float>(Width), static_cast<float>(Height));

	textureManager_.getTexture("sky").setRepeated(true);
	bgSprite_.setTexture(textureManager_.getTexture("sky"));
	bgSprite_.setTextureRect(sf::IntRect(0, 0, mapLoader_.GetMapSize().x, mapLoader_.GetMapSize().y));
}

Game::~Game()
{
	delete world_;
	world_ = nullptr;
}

void Game::loadTextures()
{
	textureManager_.addTexture("player", "Assets/player.png");
	textureManager_.addTexture("coin1", "Assets/coin1.png");
	textureManager_.addTexture("coin5", "Assets/coin5.png");
	textureManager_.addTexture("coin100", "Assets/coin100.png");
	textureManager_.addTexture("spike", "Assets/spike.png");
	textureManager_.addTexture("healthBar", "Assets/healthbar.png");
	textureManager_.addTexture("jumpPowerup", "Assets/jumpPowerup.png");
	textureManager_.addTexture("spike left", "Assets/spike left.png");
	textureManager_.addTexture("spike right", "Assets/spike right.png");
	textureManager_.addTexture("spike down", "Assets/spike down.png");
	textureManager_.addTexture("sky", "Assets/sky.png");
}

void Game::createEntities()
{
	entities_.push_back(std::unique_ptr<Entity>(new Player(sf::Vector2f(Width / 4.f, 100.f), this)));
}

void Game::createWorld()
{
	mapLoader_.Load("map.tmx");
	std::vector<tmx::MapLayer>& layers = mapLoader_.GetLayers();

	for (auto& layer : layers)
	{
		if (layer.name == "Static")
		{
			for (auto& obj : layer.objects)
			{
				auto body = tmx::BodyCreator::Add(obj, *world_);
				body->SetTransform(b2Vec2(body->GetPosition().x, -body->GetPosition().y), body->GetAngle());
				body->SetFixedRotation(true);
				body->SetUserData(new Ground(this));
				
				for (auto fixture = body->GetFixtureList(); fixture ; fixture = fixture->GetNext())
				{
					fixture->SetFriction(0.f);
				}
			} 
		}
		else if (layer.name == "Coins")
		{
			for (auto& obj : layer.objects)
			{
				int worth = 1;

				if (!obj.GetPropertyString("worth").empty())
					worth = std::stoi(obj.GetPropertyString("worth"));

				entities_.push_back(std::unique_ptr<Entity>(new Coin(sf::Vector2f(obj.GetPosition().x, obj.GetPosition().y), this, worth)));
			} 
		}
		else if (layer.name == "Spikes")
		{
			for (auto& obj : layer.objects)
			{
				std::string direction = "";

				if (!obj.GetPropertyString("direction").empty())
					direction = obj.GetPropertyString("direction");

				entities_.push_back(std::unique_ptr<Entity>(new Spike(sf::Vector2f(obj.GetPosition().x, obj.GetPosition().y), this, direction)));
			} 
		}
		else if (layer.name == "Player")
		{
			for (auto& obj : layer.objects)
			{
				SetPositionMessage msg("player", obj.GetAABB().left + obj.GetAABB().width / 2.f,
					obj.GetAABB().top - obj.GetAABB().height / 2.f);
				sendMessage(msg);
				playerStartPos_.x = msg.getPosition().x;
				playerStartPos_.y = msg.getPosition().y;
			}
		}
		else if (layer.name == "JumpPowerups")
		{
			for (auto& obj : layer.objects)
			{
				entities_.push_back(std::unique_ptr<Entity>(new JumpPowerup(sf::Vector2f(obj.GetPosition().x, obj.GetPosition().y), this)));
			} 
		}
	}

	world_->SetContactListener(new ContactListener(this));
}

void Game::handleInput()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window_.close();
			break;
		case sf::Event::LostFocus:
			hasFocus_ = false;
			break;
		case sf::Event::GainedFocus:
			hasFocus_ = true;
			break;
		default:
			break;
		}
	}

	if (hasFocus_)
	{
		for (auto& entity : entities_)
		entity->handleInput();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			shouldReset_ = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			view_.zoom(1.2f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			view_.zoom(0.8f);
	}
}

void Game::reset()
{

	SetVelocityMessage setVel("player", 0.f, 0.f);
	SetPositionMessage setPos("player", playerStartPos_);
	sendMessage(setPos);
	sendMessage(setVel);
	view_.setCenter(Width / 2.f, Height / 2.f);
	shouldReset_ = false;
}

void Game::update(sf::Time delta)
{
	if (hasFocus_)
	{
		updateView();
		
		for (auto& entity : entities_)
			entity->update(delta);

		world_->Step(TimePerFrame.asSeconds(), 6, 3);

		entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [](std::unique_ptr<Entity>& ent) { return ent->shouldRemove(); }), std::end(entities_));

		if (shouldReset_)
			reset();
	}
}

void Game::render()
{
	if (hasFocus_)
	{
		window_.clear(sf::Color::Black);

		window_.draw(bgSprite_);
		window_.setView(view_);
		window_.draw(mapLoader_);

		for (auto& entity : entities_)
			entity->render(window_);

		window_.setView(window_.getDefaultView());
		window_.draw(coinsText_);
		window_.draw(healthBar_);

		window_.display();
	}
}

void Game::updateView()
{
	GetPositionMessage msg("player");
	sendMessage(msg);
	if ((msg.getPosition().x >= Width / 2.f) && (msg.getPosition().x <= mapLoader_.GetMapSize().x - Width / 2.f))
		view_.setCenter(sf::Vector2f(msg.getPosition().x, Height / 2.f));
}

void Game::sendMessage(Message& message)
{
	
	for (auto& entity : entities_)
	{
		if (message.getTargetId() == entity->getId() || message.getTargetId() == "all")
			entity->handleMessage(message);
		if (message.getTargetId() == "all" || message.getTargetId() == "game")
			handleMessage(message);
	}
}

void Game::handleMessage(Message& message)
{
	switch (message.getType())
	{
	case GotCoinMsg:
		{
			GetAmountCoinsMessage msg("player");
			sendMessage(msg);
			coinsText_.setString("Coins: " + std::to_string(msg.getCoins()));
		}
		break;
	case HitSpikeMsg:
		{
			shouldReset_ = true;
			GetHealthMessage msg("player");
			sendMessage(msg);
			if (msg.getHealth() >= 0) // TODO: Remove this when added checks for if the player is dead.
				healthBar_.setTextureRect(sf::IntRect(0, 0, msg.getHealth() * 2.f, healthBar_.getTextureRect().height));
		}
		break;
	default:
		break;
	}
}

void Game::run()
{
	sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window_.isOpen())
    {
		sf::Time delta = clock.restart();
        timeSinceLastUpdate += delta;

        while (timeSinceLastUpdate > TimePerFrame)
        {
			timeSinceLastUpdate -= TimePerFrame;
            handleInput();
            update(TimePerFrame);
        }

        render();
	}
}