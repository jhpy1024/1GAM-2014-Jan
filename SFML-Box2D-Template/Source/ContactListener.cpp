#include "../Include/Game.hpp"
#include "../Include/Entity.hpp"
#include "../Include/GotCoinMessage.hpp"
#include "../Include/ContactListener.hpp"
#include "../Include/PlayerFootSensor.hpp"

ContactListener::ContactListener(Game* game)
	: game_(game)
{
	beginFunctions_.push_back(
	[](Entity* entity)
	{
		if (entity->getId() == "playerFootSensor")
		{
			auto sensor = static_cast<PlayerFootSensor*>(entity);
			sensor->setNumContacts(sensor->getNumContacts() + 1);
		}
	});

	beginFunctions_.push_back(
	[game](Entity* entity)
	{
		if (entity->getId().find("coin") != entity->getId().npos)
		{
			auto coin = static_cast<Coin*>(entity);
			game->sendMessage(GotCoinMessage("all", entity->getId(), *coin));
		}
	}
	);

	endFunctions_.push_back(
	[](Entity* entity)
	{
		if (entity->getId() == "playerFootSensor")
		{
			auto sensor = static_cast<PlayerFootSensor*>(entity);
			sensor->setNumContacts(sensor->getNumContacts() - 1);
		}
	});
}

void ContactListener::BeginContact(b2Contact* contact)
{
	void* fixtureData = contact->GetFixtureA()->GetUserData();
	if (fixtureData)
	{
		auto entity = static_cast<Entity*>(fixtureData);
		for (auto func : beginFunctions_)
			func(entity);
	}

	fixtureData = contact->GetFixtureB()->GetUserData();
	if (fixtureData)
	{
		auto entity = static_cast<Entity*>(fixtureData);
		for (auto func : beginFunctions_)
			func(entity);
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	void* fixtureData = contact->GetFixtureA()->GetUserData();
	if (fixtureData)
	{
		auto entity = static_cast<Entity*>(fixtureData);
		for (auto func : endFunctions_)
			func(entity);
	}

	fixtureData = contact->GetFixtureB()->GetUserData();
	if (fixtureData)
	{
		auto entity = static_cast<Entity*>(fixtureData);
		for (auto func : endFunctions_)
			func(entity);
	}
}