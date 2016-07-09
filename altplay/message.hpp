#pragma once
#include <string>

namespace altplay
{
struct message_struct {
    std::string nick{"null"}, hostmask{"null"}, ident{"null"}, prefix{"null"}, params{"null"}, command{"null"},
    target{"null"}, message{"null"};
    bool is_server_message{false};

#define ret(n) return n.c_str()
	const char *getNick() const		{ ret(nick); }
	const char *getHostmask() const { ret(hostmask); }
	const char *getIdent() const	{ ret(ident); }
	const char *getPrefix() const	{ ret(prefix); }
	const char *getParams() const	{ ret(params); }
	const char *getTarget() const	{ ret(target); }
	const char *getMessage() const	{ ret(message); }
#undef ret
};
} // end of ns altplay
