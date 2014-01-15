#ifndef PAUSE_ENTITY_MESSAGE_HPP
#define PAUSE_ENTITY_MESSAGE_HPP

#include "Message.hpp"
#include <SFML/Graphics.hpp>

class PauseEntityMessage : public Message
{
public:
	PauseEntityMessage(const std::string& targetId, const sf::Time& pauseTime);

	sf::Time getPauseTime() const { return pauseTime_; }

private:
	sf::Time pauseTime_;
};

#endif