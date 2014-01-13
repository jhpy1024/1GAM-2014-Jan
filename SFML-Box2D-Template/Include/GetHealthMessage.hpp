#ifndef GET_HEALTH_MESSAGE_HPP
#define GET_HEALTH_MESSAGE_HPP

#include "Message.hpp"

class GetHealthMessage : public Message
{
public:
	GetHealthMessage(const std::string& targetId);

	void setHealth(int health);
	int getHealth() const { return health_; }

private:
	int health_;
};

#endif