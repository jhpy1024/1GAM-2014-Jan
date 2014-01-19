#include "../Include/CannonFiredMessage.hpp"

CannonFiredMessage::CannonFiredMessage(const std::string& targetId, const sf::Vector2f& position)
	: Message(CannonFiredMsg, targetId)
	, position_(position)
{

}