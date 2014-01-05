#include "../Include/GetVelocityMessage.hpp"

GetVelocityMessage::GetVelocityMessage(const std::string& targetId)
	: Message(GetVelocityMsg, targetId)
{

}

void GetVelocityMessage::setVelocity(float vx, float vy)
{
	velocity_ = sf::Vector2f(vx, vy);
}

void GetVelocityMessage::setVelocity(const sf::Vector2f& velocity)
{
	velocity_ = velocity;
}