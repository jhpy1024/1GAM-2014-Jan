#ifndef SET_VELOCITY_MESSAGE_HPP
#define SET_VELOCITY_MESSAGE_HPP

#include "Message.hpp"
#include <SFML/Graphics.hpp>

class SetVelocityMessage : public Message
{
public:
	SetVelocityMessage(const std::string& targetId, float vx, float vy);
	SetVelocityMessage(const std::string& targetId, const sf::Vector2f& velocity);

	sf::Vector2f getVelocity() const { return velocity_; }

private:
	sf::Vector2f velocity_;
};

#endif