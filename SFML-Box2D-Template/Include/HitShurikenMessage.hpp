#ifndef HIT_SHURIKEN_MESSAGE_HPP
#define HIT_SHURIKEN_MESSAGE_HPP

#include "Message.hpp"

class HitShurikenMessage : public Message
{
public:
	HitShurikenMessage(const std::string& targetId);
};

#endif