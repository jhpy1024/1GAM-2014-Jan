#ifndef RESUMED_MESSAGE_HPP
#define RESUMED_MESSAGE_HPP

#include "Message.hpp"

enum class ResumedFrom
{
	Spikes,
};

class ResumedMessage : public Message
{
public:
	ResumedMessage(const std::string& targetId, ResumedFrom from);

	ResumedFrom getResumedFrom() const { return from_; }

private:
	ResumedFrom from_;
};

#endif