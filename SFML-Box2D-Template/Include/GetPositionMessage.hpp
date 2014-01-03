#ifndef GET_POSITION_MESSAGE_HPP
#define GET_POSITION_MESSAGE_HPP

#include "Message.hpp"
#include <SFML/Graphics.hpp>

class GetPositionMessage : public Message
{
public:
	GetPositionMessage(const std::string& targetId);

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const { return position_; }

private:
	sf::Vector2f position_;
};

#endif