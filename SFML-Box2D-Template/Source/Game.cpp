#include "../Include/Game.hpp"
#include "../Include/Coin.hpp"
#include "../Include/Spike.hpp"
#include "../Include/Enemy.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Player.hpp"
#include "../Include/Ground.hpp"
#include "../Include/Finish.hpp"
#include "../Include/CannonBall.hpp"
#include "../Include/MonkeyEnemy.hpp"
#include "../Include/JumpPowerup.hpp"
#include "../Include/tmx/tmx2box2d.h"
#include "../Include/GotCoinMessage.hpp"
#include "../Include/ResumedMessage.hpp"
#include "../Include/ContactListener.hpp"
#include "../Include/GetHealthMessage.hpp"
#include "../Include/CannonFiredMessage.hpp"
#include "../Include/GetRotationMessage.hpp"
#include "../Include/GetPositionMessage.hpp"
#include "../Include/SetPositionMessage.hpp"
#include "../Include/SetVelocityMessage.hpp"
#include "../Include/PauseEntityMessage.hpp"
#include "../Include/GetVelocityMessage.hpp"
#include "../Include/GetAmountCoinsMessage.hpp"
#include "../Include/GotJumpPowerupMessage.hpp"

#include <Thor/Math/Distributions.hpp>

Game::Game()
	: Width(1280)
	, Height(720)
	, Title("1GAM | Jan | 2014")
	, TimePerFrame(sf::seconds(1.f / 60.f))
	, Gravity(0.f, 10.f)
	, window_(sf::VideoMode(Width, Height), Title, sf::Style::Close)
	, mapLoader_("Assets/")
	, hasFocus_(true)
	, shouldReset_(false)
	, HealthBarScale(4)
	, rot(0)
	, finished_(false)
	, died_(false)
{
	setState(Play);
	loadTextures();
}

void Game::finish()
{
	finished_ = true;
}

void Game::setState(GameState state)
{
	switch (state)
	{
	case Play:
		initPlay();
		state_ = Play;
		break;
	case Menu:
		initMenu();
		state_ = Menu;
		break;
	case Win:
		initWin();
		state_ = Win;
		break;
	case Lose:
		initLose();
		state_ = Lose;
		break;
	default:
		break;
	}
}

void Game::initPlay()
{
	world_ = new b2World(Gravity);

	createEntities();
	createWorld();

	coinsFont_.loadFromFile("Assets/coinsFont.ttf");
	coinsText_.setFont(coinsFont_);
	coinsText_.setColor(sf::Color::Red);
	coinsText_.setString("Coins: 0");
	coinsText_.setCharacterSize(30);
	coinsText_.setPosition(32.f, 30.f);
	
	finishedText_.setFont(coinsFont_);
	finishedText_.setColor(sf::Color::Black);
	finishedText_.setString("You win!");
	finishedText_.setCharacterSize(30);
	finishedText_.setOrigin(finishedText_.getLocalBounds().left + finishedText_.getLocalBounds().width / 2.f,
		finishedText_.getLocalBounds().top + finishedText_.getLocalBounds().height / 2.f);
	finishedText_.setPosition(Width / 2.f, Height / 2.f);

	diedText_.setFont(coinsFont_);
	diedText_.setColor(sf::Color::Black);
	diedText_.setString("You died, you suck!");
	diedText_.setCharacterSize(30);
	diedText_.setOrigin(diedText_.getLocalBounds().left + diedText_.getLocalBounds().width / 2.f,
		diedText_.getLocalBounds().top + diedText_.getLocalBounds().height / 2.f);
	diedText_.setPosition(Width / 2.f, Height / 2.f);

	healthBar_.setTexture(textureManager_.getTexture("spriteSheet"));
	healthBar_.setTextureRect(sf::IntRect(0, 150, 400, 15));
	healthBar_.setOrigin(healthBar_.getLocalBounds().left + healthBar_.getLocalBounds().width / 2.f,
		healthBar_.getLocalBounds().top + healthBar_.getLocalBounds().height / 2.f);
	healthBar_.setPosition(Width / 2.f, 50.f);

	view_.setCenter(Width / 2.f, Height / 2.f);
	view_.setSize(static_cast<float>(Width), static_cast<float>(Height));

	bgShape_.setSize(sf::Vector2f(mapLoader_.GetMapSize().x, mapLoader_.GetMapSize().y));
	bgShape_.setFillColor(sf::Color(125, 255, 255));

	createBloodParticleSystem();
	createSmokeParticleSystem();
}

void Game::initMenu()
{

}

void Game::initWin()
{

}

void Game::initLose()
{

}

void Game::handleInputPlay()
{
	if (finished_ || died_) return;
	for (auto& entity : entities_)
		entity->handleInput();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		shouldReset_ = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		view_.zoom(1.2f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		view_.zoom(0.8f);
}

void Game::handleInputMenu()
{

}

void Game::handleInputWin()
{

}

void Game::handleInputLose()
{

}

void Game::updatePlay(sf::Time delta)
{
	if (finished_ || died_) return;

	updateView();
	bloodParticleSystem_.update(delta);
	smokeParticleSystem_.update(delta);
		
	for (auto& entity : entities_)
		entity->update(delta);

	world_->Step(TimePerFrame.asSeconds(), 6, 3);

	entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [](std::unique_ptr<Entity>& ent) { return ent->shouldRemove(); }), std::end(entities_));
	std::for_each(entitiesToAdd_.begin(), entitiesToAdd_.end(), [&](Entity* ent) { entities_.push_back(std::unique_ptr<Entity>(ent)); });
	entitiesToAdd_.clear();

	if (shouldReset_)
		reset();

	rot = (static_cast<int>(rot) + 1) % 360;
}

void Game::updateMenu(sf::Time delta)
{

}

void Game::updateWin(sf::Time delta)
{

}

void Game::updateLose(sf::Time delta)
{

}

void Game::renderPlay()
{
	window_.clear(sf::Color(1, 255, 255));

	if (finished_)
	{
		window_.setView(window_.getDefaultView());
		window_.clear(sf::Color::Green);
		window_.draw(finishedText_);
		window_.display();
		return;
	}

	if (died_)
	{
		window_.setView(window_.getDefaultView());
		window_.clear(sf::Color::Red);
		window_.draw(diedText_);
		window_.display();
		return;
	}

	window_.draw(bgShape_);
	window_.setView(view_);
	window_.draw(mapLoader_);

	for (auto& entity : entities_)
		entity->render(window_);

	window_.setView(window_.getDefaultView());
	window_.draw(coinsText_);
	window_.draw(healthBar_);

	window_.setView(view_);
	window_.draw(bloodParticleSystem_);
	window_.draw(smokeParticleSystem_);

	window_.display();
}

void Game::renderMenu()
{

}

void Game::renderWin()
{

}

void Game::renderLose()
{

}

void Game::createBloodParticleSystem()
{
	bloodParticleSystem_.setTexture(textureManager_.getTexture("spriteSheet"));
	bloodParticleSystem_.addTextureRect(sf::IntRect(0, 170, 10, 10));
	thor::ColorGradient gradient;
	gradient[0.f] = sf::Color::Red;
	gradient[0.5f] = sf::Color(255, 112, 112);
	gradient[1.f] = sf::Color(255, 181, 181);
	thor::ColorAnimation colorizer(gradient);
	thor::FadeAnimation fader(0.1f, 0.1f);
	bloodParticleSystem_.addAffector(thor::AnimationAffector(colorizer));
	bloodParticleSystem_.addAffector(thor::AnimationAffector(fader));
}

void Game::createSmokeParticleSystem()
{
	smokeParticleSystem_.setTexture(textureManager_.getTexture("spriteSheet"));
	smokeParticleSystem_.addTextureRect(sf::IntRect(0, 190, 20, 20));
	thor::ColorGradient gradient;
	gradient[0.f] = sf::Color::Black;
	gradient[0.5f] = sf::Color(77, 77, 77);
	gradient[1.f] = sf::Color(150, 150, 150);
	thor::ColorAnimation colorizer(gradient);
	thor::FadeAnimation fader(0.1f, 0.7f);
	smokeParticleSystem_.addAffector(thor::AnimationAffector(colorizer));
	smokeParticleSystem_.addAffector(thor::AnimationAffector(fader));
}

Game::~Game()
{
	delete world_;
	world_ = nullptr;
}

void Game::loadTextures()
{
	textureManager_.addTexture("spriteSheet", "Assets/spriteSheet.png");
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

				entities_.push_back(std::unique_ptr<Entity>(
					new Coin(sf::Vector2f(obj.GetPosition().x + obj.GetAABB().width / 2.f, obj.GetPosition().y + obj.GetAABB().height / 2.f), this, worth)));
			} 
		}
		else if (layer.name == "Spikes")
		{
			for (auto& obj : layer.objects)
			{
				std::string direction = "";

				if (!obj.GetPropertyString("direction").empty())
					direction = obj.GetPropertyString("direction");

				entities_.push_back(std::unique_ptr<Entity>(
					new Spike(sf::Vector2f(obj.GetPosition().x + obj.GetAABB().width / 2.f, obj.GetPosition().y + obj.GetAABB().height / 2.f), this, direction)));
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
				entities_.push_back(std::unique_ptr<Entity>(
					new JumpPowerup(sf::Vector2f(obj.GetPosition().x + obj.GetAABB().width / 2.f, obj.GetPosition().y + obj.GetAABB().height / 2.f), this)));
			} 
		}
		else if (layer.name == "Enemies")
		{
			for (auto& obj : layer.objects)
			{
				entities_.push_back(std::unique_ptr<Entity>(
					new Enemy(sf::Vector2f(obj.GetPosition().x + obj.GetAABB().width / 2.f, obj.GetPosition().y + obj.GetAABB().height / 2.f), this)));
			} 
		}
		else if (layer.name == "MonkeyEnemies")
		{
			for (auto& obj : layer.objects)
			{
				std::string direction = "left";
				float angle = 90.f;

				if (!obj.GetPropertyString("direction").empty())
					direction = obj.GetPropertyString("direction");
				if (!obj.GetPropertyString("angle").empty())
					angle = std::stof(obj.GetPropertyString("angle"));

				entities_.push_back(std::unique_ptr<Entity>(
					new MonkeyEnemy(sf::Vector2f(obj.GetPosition().x + obj.GetAABB().width / 2.f, obj.GetPosition().y + obj.GetAABB().height / 2.f), this, direction, angle)));
			} 
		}
		else if (layer.name == "Finish")
		{
			for (auto& obj : layer.objects)
			{
				entities_.push_back(std::unique_ptr<Entity>(
					new Finish(sf::Vector2f(obj.GetPosition().x + obj.GetAABB().width / 2.f, obj.GetPosition().y + obj.GetAABB().height / 2.f), this)));
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
		switch (state_)
		{
		case Play:
			handleInputPlay();
			break;
		case Menu:
			handleInputMenu();
			break;
		case Win:
			handleInputWin();
			break;
		case Lose:
			handleInputLose();
			break;
		default:
			break;
		}
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
		switch (state_)
		{
		case Play:
			updatePlay(delta);
			break;
		case Menu:
			updateMenu(delta);
			break;
		case Win:
			updateWin(delta);
			break;
		case Lose:
			updateLose(delta);
			break;
		default:
			break;
		}
	}
}

void Game::render()
{
	if (hasFocus_)
	{
		switch (state_)
		{
		case Play:
			renderPlay();
			break;
		case Menu:
			renderMenu();
			break;
		case Win:
			renderWin();
			break;
		case Lose:
			renderLose();
			break;
		default:
			break;
		}
	}
}

void Game::addEntity(Entity* entity)
{
	entitiesToAdd_.push_back(entity);
}

thor::UniversalEmitter Game::createBloodEmitter()
{
	GetPositionMessage msg("player");
	sendMessage(msg);

	thor::UniversalEmitter emitter;
	emitter.setEmissionRate(300.f);
	emitter.setParticleLifetime(sf::seconds(1.f));
	emitter.setParticlePosition(thor::Distributions::circle(sf::Vector2f(msg.getPosition().x, msg.getPosition().y), 1.f));
    emitter.setParticleRotation(thor::Distributions::uniform(0.f, 360.f));
    float xVel = static_cast<float>(rand() % 360 + 120);
    float yVel = static_cast<float>(rand() % 360 + 120);
    emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(xVel, yVel), 360));

    return emitter;
}

thor::UniversalEmitter Game::createSmokeEmitter(const sf::Vector2f& position)
{
	thor::UniversalEmitter emitter;
	emitter.setEmissionRate(300.f);
	emitter.setParticleLifetime(sf::seconds(0.4f));
	emitter.setParticlePosition(thor::Distributions::circle(sf::Vector2f(position.x, position.y), 1.f));
    emitter.setParticleRotation(thor::Distributions::uniform(0.f, 360.f));
    float xVel = static_cast<float>(rand() % 360 + 120);
    float yVel = static_cast<float>(rand() % 360 + 120);
    emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(xVel, yVel), 360));

    return emitter;
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
	case HitCannonBallMsg:
	case HitShurikenMsg:
		{	
			bloodParticleSystem_.addEmitter(createBloodEmitter(), sf::seconds(0.05f));
			shouldReset_ = true;
			GetHealthMessage msg("player");
			sendMessage(msg);
			if (msg.getHealth() > 0) 
				healthBar_.setTextureRect(sf::IntRect(0, 150, msg.getHealth() * HealthBarScale, healthBar_.getTextureRect().height));
			else
				died_ = true;
			sendMessage(PauseEntityMessage("player", sf::seconds(1.f)));
			healthBar_.setOrigin(healthBar_.getLocalBounds().left + healthBar_.getLocalBounds().width / 2.f,
				healthBar_.getLocalBounds().top + healthBar_.getLocalBounds().height / 2.f);
			healthBar_.setPosition(Width / 2.f, 50.f);
		}
		break;
	case CannonFiredMsg:
		{
			auto& msg = static_cast<CannonFiredMessage&>(message);
			smokeParticleSystem_.addEmitter(createSmokeEmitter(msg.getPosition()), sf::seconds(0.05f));
		}
		break;
	case ResumedMsg:
		{
			auto& msg = static_cast<ResumedMessage&>(message);
			
			switch (msg.getResumedFrom())
			{
			case ResumedFrom::Spikes:
				shouldReset_ = true;
				break;
			default:
				break;
			}
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