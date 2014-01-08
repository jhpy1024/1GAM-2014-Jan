#ifndef CONTACT_LISTENER_HPP
#define CONTACT_LISTENER_HPP

#include <vector>
#include <functional>
#include <Box2D/Box2D.h>

class Game;
class ContactListener : public b2ContactListener
{
public:
	ContactListener(Game* game);

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

private:
	Game* game_;
	std::vector<std::function<void(Entity*)>> beginFunctions_;
	std::vector<std::function<void(Entity*)>> endFunctions_;
};

#endif