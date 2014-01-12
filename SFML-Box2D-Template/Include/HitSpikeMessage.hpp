#ifndef HIT_SPIKE_MESSAGE_HPP
#define HIT_SPIKE_MESSAGE_HPP

#include "Message.hpp"

class HitSpikeMessage : public Message
{
public:
	HitSpikeMessage(const std::string& targetId);
};

#endif