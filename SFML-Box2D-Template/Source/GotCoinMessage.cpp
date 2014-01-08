#include "../Include/GotCoinMessage.hpp"

GotCoinMessage::GotCoinMessage(const std::string& targetId, const std::string& coinId, Coin& coin)
	: Message(GotCoinMsg, targetId)
	, coinId_(coinId)
	, coin_(coin)
{

}