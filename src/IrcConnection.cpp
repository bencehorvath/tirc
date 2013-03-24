#include "tirclib.h"



IrcConnection::IrcConnection(std::string ircHost, uint16 port, std::string nick, std::string user)
	: m_resolver(m_io_service), m_socket(m_io_service), m_sent_registering_packets(false)
{
	std::cout << "Initializing" << std::endl;

	m_server_host = ircHost;
	m_server_port = port;
	m_nick = nick;
	m_user = user;

	m_connection_status = STATUS_NOT_CONNECTED;
}

void IrcConnection::Connect()
{
	/*boost::asio::ip::tcp::resolver::query query(m_irc_host, m_irc_service);
	resolver.async_resolve(query, boost::bind(&IrcConnection::handle_resolve, shared_from_this(), boost::asio::placeholders::error
		, boost::asio::placeholders::iterator));*/

	// DNS lookup

	try
	{
		ip::tcp::resolver::query query(m_server_host, "0");
		ip::tcp::resolver::iterator endpoint_iterator = m_resolver.resolve(query);
		ip::tcp::resolver::iterator end;

		error_code error = error::host_not_found;

		while(error && endpoint_iterator != end)
		{
			auto ep = endpoint_iterator->endpoint();
			std::cout << ep.address().to_string() << ":" << ep.port() << std::endl;
			ep.port(m_server_port);
			m_socket.close();
			m_socket.connect(ep, error);
			endpoint_iterator++;
		}
		if(error)
			throw boost::system::system_error(error);

		Flush();
		m_sending = false;
		m_connection_status = STATUS_CONNECTED;

		std::cout << "Connected." << std::endl;

		AddHandler(REPLY_MOTD_END, [this](IrcMessage const& m){ this->HandleRegistered(m); });


		OnMessage = [this](std::string const& s) -> void { this->HandleData(s); };

		Receive();
		m_io_service.run();

	}
	catch(...)
	{
		m_connection_status = STATUS_NOT_CONNECTED;
		m_io_service.dispatch(OnDisconnect);
		throw;
	}

}



void IrcConnection::Flush()
{
	while(m_out_queue.size() > 0)
		m_out_queue.pop();
}

void IrcConnection::FinishSending()
{
	while(m_sending)
		m_io_service.run_one();
}

void IrcConnection::Send(uint8 priority, const std::string& message)
{
	NetMessage msg(priority, message);

	m_out_queue.push(msg);
	SendAsync();
}

void IrcConnection::SendAsync()
{
	if(/*!m_sending && */ m_out_queue.size() > 0)
	{
		std::lock_guard<std::mutex> guard(m_queue_guard);
		m_current_message = m_out_queue.top();
		m_out_queue.pop();
		boost::asio::async_write(m_socket, boost::asio::buffer(m_current_message.message), [this](error_code const& ec, std::size_t bt) { this->SendFinished(ec, bt); });

		m_sending = true;
	}
}

void IrcConnection::SendFinished(const error_code& ec, std::size_t bt)
{
	if(ec)
	{
		Disconnect();
		throw boost::system::system_error(ec);
	}

	m_sending = false;
	SendAsync();

}

void IrcConnection::Receive()
{
	boost::asio::async_read_until(m_socket, m_incoming_buffer, "\n", [this](error_code const& ec, std::size_t bt) { this->ReceiveFinished(ec,bt); });
}

void IrcConnection::ReceiveFinished(error_code const& ec, std::size_t bt)
{
	if(ec)
	{
		Disconnect();
		throw boost::system::system_error(ec);
	}

	std::lock_guard<std::mutex> guard(m_line_guard);
	std::istream is(&m_incoming_buffer);
	std::getline(is, m_lastLine);
	m_io_service.dispatch([this]() { this->OnMessage(m_lastLine); });
	Receive();

}

IrcConnection::~IrcConnection()
{
	Disconnect();
}

void IrcConnection::Disconnect()
{
	if(m_connection_status >= STATUS_CONNECTED)
	{
		m_socket.close();
		m_connection_status = STATUS_NOT_CONNECTED;
		m_io_service.dispatch(OnDisconnect);
	}
}

void IrcConnection::HandleData(std::string const& data)
{

	HandleMessage(data);

	if(!m_sent_registering_packets)
	{
		std::string nickmsg = (boost::format("%1% %2%") % REPLY_NICK % m_nick).str();
		std::string usermsg = (boost::format("%1% %2% 8 * :%1%") % REPLY_USER % m_user).str();

		std::cout << nickmsg << std::endl;
		std::cout << usermsg << std::endl << std::endl;

		Send(10, nickmsg);
		Send(9, usermsg);

		m_sent_registering_packets = true;
	}
}

void IrcConnection::WriteRaw(std::string data)
{
	boost::asio::write(m_socket, boost::asio::buffer(data));
}

void IrcConnection::AddHandler(std::string opcode, OpcodeHandler handler)
{
	m_handlers[opcode] = handler;
}

void IrcConnection::HandleRegistered(IrcMessage const& msg)
{
	m_connection_status = STATUS_REGISTERED;
	// register others

	AddHandler(REPLY_PING, [this](IrcMessage const& msg) { this->HandlePing(msg); });

}

void IrcConnection::HandlePing(IrcMessage const& msg)
{
	auto response = (boost::format("%1% %2%") % REPLY_PONG % msg.args).str();
	std::cout << response << std::endl;

	Send(10, response);
}

void IrcConnection::HandleMessage(std::string info)
{
	IrcMessage mess;

	boost::regex firstRegex("^[:](\\S+)\\s(\\S+|\\d+)\\s(\\S+)\\s(\\S+)\\s[:](.+)");
	boost::cmatch match;


	if(boost::regex_search(info.c_str(), match, firstRegex))
	{
		mess.full = info;
		mess.host = match[1];
		mess.hostmask = match[1];
		mess.opcode = match[2];
		mess.nick = match[3];
		mess.user = match[3];
		mess.channel = match[4];
		mess.args = match[5];
	}
	else
	{
		boost::regex secondRegex("^(\\S+)\\s[:](\\S+)");

		if(boost::regex_search(info.c_str(), match, secondRegex))
		{
			mess.full = info;
			mess.host = m_server_host;
			mess.hostmask = m_server_host;
			mess.opcode = match[1];
			mess.args = match[2];
		}
		else
		{
			boost::regex thirdRegex("(:\\S+)\\s(\\S+|\\d+)\\s(\\S+)\\s:(.+)");

			if(boost::regex_search(info.c_str(), match, thirdRegex))
			{
				mess.full = info;
				mess.hostmask = match[1];
				mess.opcode = match[2];
				mess.channel = match[3];
				mess.args = match[4];

				// split hostmask
				unsigned int pos = (mess.hostmask.find('!'));
				mess.nick = mess.hostmask.substr(0, pos);
				mess.host = mess.hostmask.substr(pos + 1);

				pos = (mess.host.find('@'));
				mess.user = mess.host.substr(0, pos);
				mess.host = mess.host.substr(pos + 1);
			}
			else
			{
				//throw std::exception("IRC message could not be parsed.");
				std::cerr << "Could not parse IRC message!!" << std::endl;
				return;
			}
		}

	}

	mess.pConnection = shared_from_this();

	try
	{
		int opcode = boost::lexical_cast<int>(mess.opcode);
		std::cout << "opcode: " << opcode << std::endl;
	}
	catch(boost::bad_lexical_cast&)
	{

	}

	auto it = m_handlers.find(mess.opcode);
	if(it == m_handlers.end())
	{
		std::cout << "Unhandled opcode: " << mess.opcode << std::endl;
		return;
	}

	(it->second)(mess); // call the handler
}

void IrcConnection::JoinChannel(std::string channel)
{
	if(m_connection_status < STATUS_REGISTERED)
		return;

	if(!boost::starts_with(channel, "#"))
		channel.insert(0, 1, '#');

	Send(1, (boost::format("%1% %2%") % REPLY_JOIN % channel).str());
}



