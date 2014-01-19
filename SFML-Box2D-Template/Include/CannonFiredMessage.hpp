#ifndef CANNON_FIRED_MESSAGE_HPP
#define CANNON_FIRED_MESSAGE_HPP

#include "Message.hpp"
#include <SFML/Graphics.hpp>

class CannonFiredMessage : public Message
{
public:
	CannonFiredMessage(const std::string& targetId, const sf::Vector2f& position);

	sf::Vector2f getPosition() const { return position_; }

private:
	sf::Vector2f position_;
};

#endif