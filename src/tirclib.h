#ifndef _HDR_TIRCLIB_H
#define _HDR_TIRCLIB_H

#if defined(HAVE_CONFIG)
#include "tirc_config.h"
#endif

#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/cstdint.hpp>

#include <functional>
#include <memory>
#include <unordered_map>
#include <mutex>



#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// GCC as of yet (4.8 @ 2013/03/22 SVN) has yet to provide extended ECMAScript regex capabilites.
// fall back to Boost.
#include <boost/regex.hpp>



typedef boost::uint8_t uint8;
typedef boost::uint16_t uint16;
typedef boost::uint32_t uint32;
typedef boost::uint64_t uint64;

typedef	boost::int8_t int8;
typedef boost::int16_t int16;
typedef boost::int32_t int32;
typedef boost::int64_t int64;

// tirclib
//

#include "IrcMessage.h"
#include "IrcReplies.h"
#include "IrcConnection.h"

#endif
