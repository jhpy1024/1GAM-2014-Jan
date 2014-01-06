#include "../Include/PlayerFootSensor.hpp"

PlayerFootSensor::PlayerFootSensor(Game* game)
	: Entity(sf::Vector2f(), game, "playerFootSensor")
	, numContacts_(0)
{

}

// -- Functions empty by default --
void PlayerFootSensor::handleInput()
{ }
void PlayerFootSensor::update(sf::Time delta)
{ }
void PlayerFootSensor::render(sf::RenderWindow& window)
{ }
void PlayerFootSensor::handleMessage(Message& message)
{ }

void PlayerFootSensor::setNumContacts(int numContacts)
{
	numContacts_ = numContacts;
}