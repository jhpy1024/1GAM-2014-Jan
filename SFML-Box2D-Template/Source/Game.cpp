#include "../Include/Game.hpp"
#include "../Include/Coin.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Player.hpp"
#include "../Include/tmx/tmx2box2d.h"
#include "../Include/ContactListener.hpp"
#include "../Include/GetPositionMessage.hpp"
#include "../Include/SetPositionMessage.hpp"
#include "../Include/SetVelocityMessage.hpp"
#include "../Include/GetVelocityMessage.hpp"

Game::Game()
	: Width(1280)
	, Height(720)
	, Title("1GAM | Jan | 2014")
	, TimePerFrame(sf::seconds(1.f / 60.f))
	, Gravity(0.f, 10.f)
	, window_(sf::VideoMode(Width, Height), Title, sf::Style::Close)
	, world_(new b2World(Gravity))
	, mapLoader_("Assets/")
{
	createEntities();
	createWorld();
	loadTextures();
	
	view_.setSize(static_cast<float>(Width), static_cast<float>(Height));
}

void Game::loadTextures()
{
	textureManager_.addTexture("player", "Assets/player.png");
	textureManager_.addTexture("coin", "Assets/coin.png");
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
				
				for (auto fixture = body->GetFixtureList(); fixture ; fixture = fixture->GetNext())
				{
					fixture->SetFriction(0.f);
				}
			} 
		} else if (layer.name == "Coins")
		{
			for (auto& obj : layer.objects)
			{
				
				entities_.push_back(std::unique_ptr<Entity>(new Coin(sf::Vector2f(obj.GetPosition().x, obj.GetPosition().y), this)));
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
		if (event.type == sf::Event::Closed)
			window_.close();
	}

	for (auto& entity : entities_)
		entity->handleInput();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		SetVelocityMessage setVel("player", 0.f, 0.f);
		SetPositionMessage setPos("player", Width / 4.f, 100.f);
		sendMessage(setPos);
		sendMessage(setVel);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		view_.zoom(1.2f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		view_.zoom(0.8f);
}

void Game::update(sf::Time delta)
{
	updateView();

	for (auto& entity : entities_)
		entity->update(delta);

	world_->Step(TimePerFrame.asSeconds(), 6, 3);
}

void Game::render()
{
	window_.clear(sf::Color::Black);

	window_.setView(view_);
	window_.draw(mapLoader_);

	for (auto& entity : entities_)
		entity->render(window_);

	window_.display();
}

void Game::updateView()
{
	GetPositionMessage msg("player");
	sendMessage(msg);
	view_.setCenter(sf::Vector2f(msg.getPosition().x, Height / 2.f));
}

void Game::sendMessage(Message& message)
{
	for (auto& entity : entities_)
	{
		if (message.getTargetId() == entity->getId() || message.getTargetId() == "all")
			entity->handleMessage(message);
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