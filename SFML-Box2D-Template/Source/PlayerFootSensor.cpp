#include "../Include/PlayerFootSensor.hpp"

PlayerFootSensor::PlayerFootSensor(Game* game)
	: Entity(sf::Vector2f(), game, "playerFootSensor")
	, numContacts_(0)
{

}

// -- Functions empty by default --
void PlayerFootSensor::handleInput()
{ }
void PlayerFootSensor::update(sf::Time)
{ }
void PlayerFootSensor::render(sf::RenderWindow&)
{ }
void PlayerFootSensor::handleMessage(Message&)
{ }

void PlayerFootSensor::setNumContacts(int numContacts)
{
	numContacts_ = numContacts;
}