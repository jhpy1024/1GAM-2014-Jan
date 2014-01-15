#include "../Include/ResumedMessage.hpp"

ResumedMessage::ResumedMessage(const std::string& targetId, ResumedFrom from)
	: Message(ResumedMsg, targetId)
	, from_(from)
{

}