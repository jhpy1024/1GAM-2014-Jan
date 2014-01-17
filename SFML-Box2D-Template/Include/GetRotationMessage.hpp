#ifndef GET_ROTATION_MESSAGE_HPP
#define GET_ROTATION_MESSAGE_HPP

#include "Message.hpp"

class GetRotationMessage : public Message
{
public:
	GetRotationMessage(const std::string& targetId);

	void setRotation(float rotation);
	float getRotation() const { return rotation_; }

private:
	float rotation_;
};

#endif