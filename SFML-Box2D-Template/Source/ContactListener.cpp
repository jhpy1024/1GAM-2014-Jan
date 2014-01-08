#include "../Include/Game.hpp"
#include "../Include/Entity.hpp"
#include "../Include/ContactListener.hpp"
#include "../Include/PlayerFootSensor.hpp"

ContactListener::ContactListener(Game* game)
	: game_(game)
{

}

void ContactListener::BeginContact(b2Contact* contact)
{
	void* fixtureData = contact->GetFixtureA()->GetUserData();
	if (fixtureData)
	{
		auto entity = static_cast<Entity*>(fixtureData);
		std::printf("Contact (A) - Entity ID: %s\n", entity->getId().c_str());
		if (entity->getId() == "playerFootSensor")
		{
			auto sensor = static_cast<PlayerFootSensor*>(entity);
			sensor->setNumContacts(sensor->getNumContacts() + 1);
		}
	}

	fixtureData = contact->GetFixtureB()->GetUserData();
	if (fixtureData)
	{
		auto entity = static_cast<Entity*>(fixtureData);
		std::printf("Contact (B) - Entity ID: %s\n", entity->getId().c_str());
		if (entity->getId() == "playerFootSensor")
		{
			auto sensor = static_cast<PlayerFootSensor*>(entity);
			sensor->setNumContacts(sensor->getNumContacts() + 1);
		}
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	void* fixtureData = contact->GetFixtureA()->GetUserData();
	if (fixtureData)
	{
		auto entity = static_cast<Entity*>(fixtureData);
		if (entity->getId() == "playerFootSensor")
		{
			auto sensor = static_cast<PlayerFootSensor*>(entity);
			sensor->setNumContacts(sensor->getNumContacts() - 1);
		}
	}

	fixtureData = contact->GetFixtureB()->GetUserData();
	if (fixtureData)
	{
		auto entity = static_cast<Entity*>(fixtureData);
		if (entity->getId() == "playerFootSensor")
		{
			auto sensor = static_cast<PlayerFootSensor*>(entity);
			sensor->setNumContacts(sensor->getNumContacts() - 1);
		}
	}
}