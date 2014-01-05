#ifndef GET_VELOCITY_MESSAGE_HPP
#define GET_VELOCITY_MESSAGE_HPP

#include "Message.hpp"
#include <SFML/Graphics.hpp>

class GetVelocityMessage : public Message
{
public:
	GetVelocityMessage(const std::string& targetId);

	void setVelocity(float vx, float vy);
	void setVelocity(const sf::Vector2f& velocity);
	sf::Vector2f getVelocity() const { return velocity_; }

private:
	sf::Vector2f velocity_;
};

#endif