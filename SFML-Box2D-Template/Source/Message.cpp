#include "../Include/Message.hpp"

Message::Message(MessageType type, const std::string& targetId)
	: type_(type)
	, targetId_(targetId)
{

}