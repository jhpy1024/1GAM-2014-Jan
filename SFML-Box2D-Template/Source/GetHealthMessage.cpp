#include "../Include/GetHealthMessage.hpp"

GetHealthMessage::GetHealthMessage(const std::string& targetId)
	: Message(GetHealthMsg, targetId)
	, health_(100)
{

}

void GetHealthMessage::setHealth(int health)
{
	health_ = health;
}