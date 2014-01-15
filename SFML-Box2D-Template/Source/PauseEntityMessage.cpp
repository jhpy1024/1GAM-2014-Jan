#include "../Include/PauseEntityMessage.hpp"

PauseEntityMessage::PauseEntityMessage(const std::string& targetId, const sf::Time& pauseTime)
	: Message(PauseEntityMsg, targetId)
	, pauseTime_(pauseTime)
{

}