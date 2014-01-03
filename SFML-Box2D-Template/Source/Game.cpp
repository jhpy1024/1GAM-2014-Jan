#include "../Include/Game.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Player.hpp"

Game::Game()
	: Width(1280)
	, Height(720)
	, Title("1GAM | Jan | 2014")
	, TimePerFrame(sf::seconds(1.f / 60.f))
	, Gravity(0.f, 10.f)
	, window_(sf::VideoMode(Width, Height), Title)
	, world_(new b2World(Gravity))
	, mapLoader_("Assets/")
{
	entities_.push_back(std::unique_ptr<Entity>(new Player(sf::Vector2f(Width / 2.f, Height / 2.f), this)));

	b2BodyDef bodyDef;
	bodyDef.position.Set(pixelsToMeters(Width / 2.f), pixelsToMeters(Height * 0.95f));
	bodyDef.type = b2_staticBody;

	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(Width / 2.f), pixelsToMeters(20 / 2.f));

	groundBody_ = world_->CreateBody(&bodyDef);
	groundBody_->CreateFixture(&shape, 1.f);

	groundShape_.setFillColor(sf::Color::Green);
	groundShape_.setSize(sf::Vector2f(Width, 20.f));
	groundShape_.setOrigin(groundShape_.getLocalBounds().left + groundShape_.getLocalBounds().width / 2.f, 
		groundShape_.getLocalBounds().top + groundShape_.getLocalBounds().height / 2.f);
	groundShape_.setPosition(Width / 2.f, Height * 0.95f);

	mapLoader_.Load("map.tmx");


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
}

void Game::update(sf::Time delta)
{
	for (auto& entity : entities_)
		entity->update(delta);

	world_->Step(TimePerFrame.asSeconds(), 6, 3);
}

void Game::render()
{
	window_.clear(sf::Color::Red);

	window_.draw(mapLoader_);
	mapLoader_.Draw(window_, tmx::MapLayer::Debug);

	for (auto& entity : entities_)
		entity->render(window_);

	window_.draw(groundShape_);
	window_.display();
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