/*  Copyright (C) 2011 Twl
(http://www.github.com/twl)

Project: http://www.github.com/twl/kyra
*/

#ifndef _HDR_TIRCLIB_IRC_MESSAGE
#define _HDR_TIRCLIB_IRC_MESSAGE

#include "tirclib.h"

class IrcConnection;

struct IrcMessage
{
	std::string hostmask;
	std::string opcode;
	std::string channel;
	std::string args;

	// split from :abyss!abyss@abyss.the.host
	std::string nick; // abyss
	std::string user; // abyss
	std::string host; // abyss.the.host

	std::string full;
	std::shared_ptr<IrcConnection> pConnection;

};

/**
 * Stores network messages, with a priority if supplied.
 */
struct NetMessage
{
	NetMessage()
	{
		priority = 0;
		message = "";
	}

	NetMessage(int priority, std::string msg)
	{
		this->priority = priority;

		if(!boost::algorithm::ends_with(msg, "\r\n"))
			message = msg + "\r\n";
	}

	uint8 priority;
	std::string message;

	bool operator <(const NetMessage& other) const
	{
		return priority < other.priority;
	}
};


#endif
