#include "../Include/HitSpikeMessage.hpp"

HitSpikeMessage::HitSpikeMessage(const std::string& targetId)
	: Message(HitSpikeMsg, targetId)
{

}