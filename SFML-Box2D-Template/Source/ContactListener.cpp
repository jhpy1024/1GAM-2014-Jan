#include "../Include/Game.hpp"
#include "../Include/Entity.hpp"
#include "../Include/CannonBall.hpp"
#include "../Include/GotCoinMessage.hpp"
#include "../Include/HitSpikeMessage.hpp"
#include "../Include/ContactListener.hpp"
#include "../Include/PlayerFootSensor.hpp"
#include "../Include/HitCannonBallMessage.hpp"
#include "../Include/GotJumpPowerupMessage.hpp"

ContactListener::ContactListener(Game* game)
	: game_(game)
{
	// playerFootSensor vs ground
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
	
	// player vs coin
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

	// player vs cannon ball
	beginFunctions_.push_back
	(
	[game](void* bodyUserDataA, void* bodyUserDataB)
	{
		if (bodyUserDataA && bodyUserDataB)
		{
			auto entityA = static_cast<Entity*>(bodyUserDataA);
			auto entityB = static_cast<Entity*>(bodyUserDataB);

			if (entityA->getId().find("cannonBall") != entityA->getId().npos && entityB->getId() == "player")
			{
				auto cannonBall = static_cast<CannonBall*>(entityA);
				HitCannonBallMessage msg("all", *cannonBall);
				game->sendMessage(msg);
			}
			else if (entityA->getId() == "player" && entityB->getId().find("cannonBall") != entityB->getId().npos)
			{
				auto cannonBall = static_cast<CannonBall*>(entityB);
				HitCannonBallMessage msg("all", *cannonBall);
				game->sendMessage(msg);
			}
		}
	}
	);

	// player vs jump powerup 
	beginFunctions_.push_back
	(
	[game](void* bodyUserDataA, void* bodyUserDataB)
	{
		if (bodyUserDataA && bodyUserDataB)
		{
			auto entityA = static_cast<Entity*>(bodyUserDataA);
			auto entityB = static_cast<Entity*>(bodyUserDataB);

			if (entityA->getId().find("jumpPowerup") != entityA->getId().npos && entityB->getId() == "player")
			{
				auto powerup = static_cast<JumpPowerup*>(entityA);
				GotJumpPowerupMessage msg("all", *powerup);
				game->sendMessage(msg);
			}
			else if (entityA->getId() == "player" && entityB->getId().find("jumpPowerup") != entityB->getId().npos)
			{
				auto powerup = static_cast<JumpPowerup*>(entityB);
				GotJumpPowerupMessage msg("all", *powerup);
				game->sendMessage(msg);
			}
		}
	}
	);

	// player vs spike
	beginFunctions_.push_back
	(
	[game](void* bodyUserDataA, void* bodyUserDataB)
	{
		if (bodyUserDataA && bodyUserDataB)
		{
			auto entityA = static_cast<Entity*>(bodyUserDataA);
			auto entityB = static_cast<Entity*>(bodyUserDataB);

			bool hitSpike = false;

			if (entityA->getId() == "player" && entityB->getId() == "spike")
			{
				hitSpike = true;
			}
			else if (entityA->getId() == "spike" && entityB->getId() == "player")
			{
				hitSpike = true;
			}

			if (hitSpike)
				game->sendMessage(HitSpikeMessage("all"));
		}
	}
	);

	// cannonball vs ground
	beginFunctions_.push_back
	(
	[game](void* bodyUserDataA, void* bodyUserDataB)
	{
		if (bodyUserDataA && bodyUserDataB)
		{
			auto entityA = static_cast<Entity*>(bodyUserDataA);
			auto entityB = static_cast<Entity*>(bodyUserDataB);

			if (entityA->getId().find("cannonBall") != entityA->getId().npos && entityB->getId() == "ground")
			{
				auto cannonBall = static_cast<CannonBall*>(entityA);
				cannonBall->remove();
			}
			else if (entityA->getId() == "ground" && entityB->getId().find("cannonBall") != entityB->getId().npos)
			{
				auto cannonBall = static_cast<CannonBall*>(entityB);
				cannonBall->remove();
			}
		}
	}
	);

	// [end] playerFootSensor vs ground
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