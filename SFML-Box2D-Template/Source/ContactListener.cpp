#include "../Include/Game.hpp"
#include "../Include/Entity.hpp"
#include "../Include/GotCoinMessage.hpp"
#include "../Include/ContactListener.hpp"
#include "../Include/PlayerFootSensor.hpp"

ContactListener::ContactListener(Game* game)
	: game_(game)
{
	beginFunctions_.push_back
	(
	[](void* bodyUserDataA, void* bodyUserDataB)
	{
		if (bodyUserDataA && bodyUserDataB)
		{
			auto entityA = static_cast<Entity*>(bodyUserDataA);
			auto entityB = static_cast<Entity*>(bodyUserDataB);

			if (entityA->getId() == "playerFootSensor" && entityB->getId() == "ground")
			{
				auto sensor = static_cast<PlayerFootSensor*>(entityA);
				sensor->setNumContacts(sensor->getNumContacts() + 1);
			}
			else if (entityA->getId() == "ground" && entityB->getId() == "playerFootSensor")
			{
				auto sensor = static_cast<PlayerFootSensor*>(entityB);
				sensor->setNumContacts(sensor->getNumContacts() + 1);
			}
		}
	}
	);
	
	beginFunctions_.push_back
	(
	[game](void* bodyUserDataA, void* bodyUserDataB)
	{
		if (bodyUserDataA && bodyUserDataB)
		{
			auto entityA = static_cast<Entity*>(bodyUserDataA);
			auto entityB = static_cast<Entity*>(bodyUserDataB);

			if (entityA->getId().find("coin") != entityA->getId().npos && entityB->getId() == "player")
			{
				auto coin = static_cast<Coin*>(entityA);
				game->sendMessage(GotCoinMessage("all", entityA->getId(), *coin));
			}
			else if (entityA->getId() == "player" && entityB->getId().find("coin") != entityB->getId().npos)
			{
				auto coin = static_cast<Coin*>(entityB);
				game->sendMessage(GotCoinMessage("all", entityB->getId(), *coin));
			}
		}
	}
	);

	endFunctions_.push_back
	(
	[](void* bodyUserDataA, void* bodyUserDataB)
	{
		if (bodyUserDataA && bodyUserDataB)
		{

			auto entityA = static_cast<Entity*>(bodyUserDataA);
			auto entityB = static_cast<Entity*>(bodyUserDataB);

			if (entityA->getId() == "playerFootSensor" && entityB->getId() == "ground")
			{
				auto sensor = static_cast<PlayerFootSensor*>(entityA);
				sensor->setNumContacts(sensor->getNumContacts() - 1);
			}
			else if (entityA->getId() == "ground" && entityB->getId() == "playerFootSensor")
			{
				auto sensor = static_cast<PlayerFootSensor*>(entityB);
				sensor->setNumContacts(sensor->getNumContacts() - 1);
			}
		}
	}
	);
}

void ContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	for (auto func : beginFunctions_)
		func(fixtureA->GetBody()->GetUserData(), fixtureB->GetBody()->GetUserData());
}

void ContactListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	for (auto func : endFunctions_)
		func(fixtureA->GetBody()->GetUserData(), fixtureB->GetBody()->GetUserData());
}