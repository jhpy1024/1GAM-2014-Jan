#include "../Include/GetPositionMessage.hpp"

GetPositionMessage::GetPositionMessage(const std::string& targetId)
	: Message(GetPositionMsg, targetId)
{

}

void GetPositionMessage::setPosition(float x, float y)
{
	position_ = sf::Vector2f(x, y);
}

void GetPositionMessage::setPosition(const sf::Vector2f& position)
{
	position_ = position;
}