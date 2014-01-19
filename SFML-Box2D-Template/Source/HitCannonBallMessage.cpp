#include "../Include/HitCannonBallMessage.hpp"

HitCannonBallMessage::HitCannonBallMessage(const std::string& targetId, CannonBall& cannonBall)
	: Message(HitCannonBallMsg, targetId)
	, cannonBall_(cannonBall)
{

}