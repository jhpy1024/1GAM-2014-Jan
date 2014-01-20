#ifndef SHOT_ENEMY_MESSAGE_HPP
#define SHOT_ENEMY_MESSAGE_HPP

#include "Enemy.hpp"
#include "Message.hpp"

class ShotEnemyMessage : public Message
{
public:
	ShotEnemyMessage(const std::string& targetId, Enemy& enemy);

	Enemy& getEnemy() { return enemy_; }

private:
	Enemy& enemy_;
};

#endif