#include "../Include/SetPositionMessage.hpp"

SetPositionMessage::SetPositionMessage(const std::string& targetId, float x, float y)
	: Message(SetPositionMsg, targetId)
	, position_(x, y)
{

}

SetPositionMessage::SetPositionMessage(const std::string& targetId, const sf::Vector2f& position)
	: Message(SetPositionMsg, targetId)
	, position_(position)
{

}