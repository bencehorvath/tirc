#ifndef _HDR_TIRCLIB_IRC_REPLIES
#define _HDR_TIRCLIB_IRC_REPLIES

#include "tirclib.h"

static const std::string REPLY_SUCCESSFUL_AUTH = "001";
static const std::string REPLY_MOTD = "372";
static const std::string REPLY_MOTD_START = "375";
static const std::string REPLY_MOTD_END = "376";
static const std::string REPLY_NOTICE = "NOTICE";
static const std::string REPLY_PRIVMSG = "PRIVMSG";
static const std::string REPLY_NICK = "NICK";
static const std::string REPLY_USER = "USER";
static const std::string REPLY_JOIN = "JOIN";
static const std::string REPLY_LEAVE = "PART";
static const std::string REPLY_QUIT = "QUIT";

static const std::string REPLY_PING = "PING";
static const std::string REPLY_PONG = "PONG";

static const std::string REPLY_KILL = "KILL";
static const std::string REPLY_KICK = "KICK";
static const std::string REPLY_MODE = "MODE";
static const std::string REPLY_ERROR_CANNOT_SEND_TO_CHANNEL = "404";
static const std::string REPLY_ERROR_NICK_IN_USE = "433";
static const std::string REPLY_ERROR_BANNED_FROM_CHANNEL = "474";
static const std::string REPLY_ERROR_CHANNEL_PASSWORD_REQUIRED = "475";

static const std::string REPLY_OPEATORS_ONLINE = "252";


static const std::string REPLY_NAME = "353";
static const std::string REPLY_END_OF_NAMES = "366";
static const std::string REPLY_ERROR_NO_TEXT_TO_SEND = "412";
static const std::string REPLY_ERROR_ALREADY_REGISTERED = "462";
static const std::string REPLY_ERROR_NOT_REGISTERED = "451";
static const std::string REPLY_WHOIS_USER = "311";
static const std::string REPLY_WHOIS_SERVER = "312";
static const std::string REPLY_WHOIS_IDLE = "317";
static const std::string REPLY_WHOIS_ENDOF_WHOIS = "318";
static const std::string REPLY_WHOIS_CHANNELS = "319";
static const std::string REPLY_WHO = "352";
static const std::string REPLY_ENDOFWHO = "315";


#endif
