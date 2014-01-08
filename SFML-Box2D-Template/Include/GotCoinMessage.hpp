#ifndef GOT_COIN_MESSAGE_HPP
#define GOT_COIN_MESSAGE_HPP

#include "Coin.hpp"
#include "Message.hpp"

class GotCoinMessage : public Message
{
public:
	GotCoinMessage(const std::string& targetId, const std::string& coinId, Coin& coin);

	Coin& getCoin() { return coin_; }
	std::string getCoinId() const { return coinId_; }

private:
	Coin& coin_;
	std::string coinId_;
};

#endif