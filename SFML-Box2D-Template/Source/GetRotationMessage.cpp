#include "../Include/GetRotationMessage.hpp"

GetRotationMessage::GetRotationMessage(const std::string& targetId)
	: Message(GetRotationMsg, targetId)
{

}

void GetRotationMessage::setRotation(float rotation)
{
	rotation_ = rotation;
}