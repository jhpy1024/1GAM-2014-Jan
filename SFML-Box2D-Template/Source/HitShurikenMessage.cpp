#include "../Include/HitShurikenMessage.hpp"

HitShurikenMessage::HitShurikenMessage(const std::string& targetId)
	: Message(HitShurikenMsg, targetId)
{

}