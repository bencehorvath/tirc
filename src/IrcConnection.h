/*  Copyright (C) 2011 Twl
(http://www.github.com/twl)

Project: http://www.github.com/twl/kyra
*/

#ifndef _HDR_TIRCLIB_IRC_CONNECTION
#define _HDR_TIRCLIB_IRC_CONNECTION

#include "tirclib.h"


/*struct status
{
	static const int not_connected = 0;
	static const int resolving = 1;
	static const int connected = 2;
	static const int registering = 3;
	static const int registered = 4;
};*/


using boost::system::error_code;
using namespace boost::asio;

struct IrcMessage;

typedef std::function<void (IrcMessage const&)> OpcodeHandler;

class IrcConnection : public std::enable_shared_from_this<IrcConnection>
{

public:

	IrcConnection(std::string ircHost, uint16 port, std::string nick, std::string user);
	~IrcConnection();

	std::string ServerHost() const { return m_server_host; }
	std::string& ServerHost() { return m_server_host; }

	int Status() const { return m_connection_status; }

	void WriteRaw(std::string data);

	void Connect();
	void Disconnect();
	void FinishSending();
	void Send(uint8 priority, std::string const& message);
	void Flush();

	//void run();

	std::function<void ()> OnConnect;
	std::function<void (const std::string&)> OnMessage;
	std::function<void ()> OnDisconnect;

	void AddHandler(std::string opcode, OpcodeHandler handler);

	void JoinChannel(std::string channel);

protected:
	void HandleResolve(const error_code&, ip::tcp::resolver::iterator);
	void HandleConnect(const error_code&);
	void HandleRead(const error_code&, std::size_t);

	void HandleData(std::string const&);

	void HandleMessage(std::string);

	void HandleRegistered(const IrcMessage&);
	void HandlePing(const IrcMessage&);
private:

	enum CONNECTION_STATUS
	{
		STATUS_NOT_CONNECTED = 0,
		STATUS_RESOLVING = 1,
		STATUS_CONNECTED,
		STATUS_REGISTERING,
		STATUS_REGISTERED
	};

	void Receive();
	void ReceiveFinished(const error_code& ec, std::size_t bt);
	void SendAsync();
	void SendFinished(const error_code& ec, std::size_t bt);

	boost::asio::io_service m_io_service;
	ip::tcp::resolver m_resolver;
	ip::tcp::socket m_socket;
	boost::asio::streambuf m_incoming_buffer;

	std::priority_queue<NetMessage> m_out_queue;
	std::mutex m_queue_guard;

	NetMessage m_current_message;

	std::string m_server_host;
	uint16 m_server_port;
	std::string m_nick;
	std::string m_user;

	std::string m_lastLine;
	std::mutex m_line_guard;
	bool m_sending;

	CONNECTION_STATUS m_connection_status;
	bool m_sent_registering_packets;

	std::unordered_map<std::string, OpcodeHandler> m_handlers;

};

#endif
