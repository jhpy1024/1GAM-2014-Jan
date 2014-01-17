#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

enum MessageType
	{
		SetPositionMsg,
		GetPositionMsg,
		GetVelocityMsg,
		SetVelocityMsg,
		GotCoinMsg,
		GetAmountCoinsMsg,
		HitSpikeMsg,
		GetHealthMsg,
		GotJumpPowerupMsg,
		PauseEntityMsg,
		ResumedMsg,
		GetRotationMsg,
	};

class Message
{
public:
	Message(MessageType type, const std::string& targetId);

	std::string getTargetId() const { return targetId_; }
	MessageType getType() const { return type_; }

private:
	std::string targetId_;
	MessageType type_;
};

#endif