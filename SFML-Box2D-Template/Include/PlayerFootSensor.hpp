#ifndef PLAYER_FOOT_SENSOR_HPP
#define PLAYER_FOOT_SENSOR_HPP

#include "Entity.hpp"

class PlayerFootSensor : public Entity
{
public:
	PlayerFootSensor(Game* game);

	void handleInput() override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void handleMessage(Message& message) override;

	void setNumContacts(int numContacts);
	int getNumContacts() const { return numContacts_; }

private:
	int numContacts_;
};

#endif