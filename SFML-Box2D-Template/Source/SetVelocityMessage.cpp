#include "../Include/SetVelocityMessage.hpp"

SetVelocityMessage::SetVelocityMessage(const std::string& targetId, float vx, float vy)
	: Message(SetVelocityMsg, targetId)
	, velocity_(vx, vy)
{

}

SetVelocityMessage::SetVelocityMessage(const std::string& targetId, const sf::Vector2f& velocity)
	: Message(SetVelocityMsg, targetId)
	, velocity_(velocity)
{

}