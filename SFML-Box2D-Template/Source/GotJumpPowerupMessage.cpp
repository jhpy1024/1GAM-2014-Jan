#include "../Include/GotJumpPowerupMessage.hpp"

GotJumpPowerupMessage::GotJumpPowerupMessage(const std::string& targetId, JumpPowerup& powerup)
	: Message(GotJumpPowerupMsg, targetId)
	, powerup_(powerup)
{

}