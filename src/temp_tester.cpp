#include "tirclib.h"

int main()
{
	std::cout << "TIrcLib tester app. Checking on rizon." << std::endl;

	std::shared_ptr<IrcConnection> connection(new IrcConnection("irc.rizon.net", 6667, "tirctest", "tirctest"));

	connection->AddHandler(REPLY_PRIVMSG, [](IrcMessage const& msg) {
		std::cout << msg.full << std::endl; });
	connection->Connect();
	return 0;
}
