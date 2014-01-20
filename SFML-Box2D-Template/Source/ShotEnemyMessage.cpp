#include "../Include/ShotEnemyMessage.hpp"

ShotEnemyMessage::ShotEnemyMessage(const std::string& targetId, Enemy& enemy)
	: Message(ShotEnemyMsg, targetId)
	, enemy_(enemy)
{

}