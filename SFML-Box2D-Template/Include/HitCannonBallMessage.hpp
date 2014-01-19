#ifndef HIT_CANNON_BALL_MESSAGE_HPP
#define HIT_CANNON_BALL_MESSAGE_HPP

#include "Message.hpp"
#include "CannonBall.hpp"

class HitCannonBallMessage : public Message
{
public:
	HitCannonBallMessage(const std::string& targetId, CannonBall& cannonBall);
	
	CannonBall& getCannonBall() { return cannonBall_; }

private:
	CannonBall& cannonBall_;
};

#endif