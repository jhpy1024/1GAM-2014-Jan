#ifndef GOT_JUMP_POWERUP_MESSAGE_HPP
#define GOT_JUMP_POWERUP_MESSAGE_HPP

#include "Message.hpp"
#include "JumpPowerup.hpp"

class GotJumpPowerupMessage : public Message
{
public:
	GotJumpPowerupMessage(const std::string& targetId, JumpPowerup& powerup);

	JumpPowerup& getPowerup() const { return powerup_; }

private:
	JumpPowerup& powerup_;
};

#endif