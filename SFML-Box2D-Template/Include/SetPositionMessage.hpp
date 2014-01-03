#ifndef SET_POSITION_MESSAGE_HPP
#define SET_POSITION_MESSAGE_HPP

#include "Message.hpp"
#include <SFML/Graphics.hpp>

class SetPositionMessage : public Message
{
public:
	SetPositionMessage(const std::string& targetId, float x, float y);
	SetPositionMessage(const std::string& targetId, const sf::Vector2f& position);

	sf::Vector2f getPosition() const { return position_; }

private:
	sf::Vector2f position_;
};

#endif