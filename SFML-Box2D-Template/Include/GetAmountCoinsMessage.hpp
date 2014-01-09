#ifndef GET_AMOUNT_COINS_MESSAGE_HPP
#define GET_AMOUNT_COINS_MESSAGE_HPP

#include "Message.hpp"

class GetAmountCoinsMessage : public Message
{
public:
	GetAmountCoinsMessage(const std::string& targetId);

	void setCoins(int coins);
	int getCoins() const { return coins_; }

private:
	int coins_;
};

#endif