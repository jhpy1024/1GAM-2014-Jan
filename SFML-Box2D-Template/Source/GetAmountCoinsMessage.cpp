#include "../Include/GetAmountCoinsMessage.hpp"

GetAmountCoinsMessage::GetAmountCoinsMessage(const std::string& targetId)
	: Message(GetAmountCoinsMsg, targetId)
	, coins_(0)
{

}

void GetAmountCoinsMessage::setCoins(int coins)
{
	coins_ = coins;
}